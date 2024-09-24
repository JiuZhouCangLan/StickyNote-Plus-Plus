#ifndef NATIVEWINDOWTEMPLATE_H
#define NATIVEWINDOWTEMPLATE_H

#include <QWidget>
#include <QTimer>
#include <QPointer>
#include <QSet>
#include <QEvent>
#include <QApplication>
#include <QScreen>
#include <QLabel>
#include <QtDebug>
#include <windows.h>
#include <WinUser.h>
#include <windowsx.h>
#include <dwmapi.h>
#include <iostream>
#include <objidl.h> // Fixes error C2504: 'IUnknown' : base class undefined

template<typename T>
class NativeWindowTemplate : public T
{
public:
    template<typename... Ts>
    explicit NativeWindowTemplate(Ts... args) : T(args...)
    {
        setResizeable(m_bResizeable);

        m_forceUpdateTimer.setSingleShot(true);
        m_forceUpdateTimer.setInterval(0);
        QObject::connect(&m_forceUpdateTimer, &QTimer::timeout, this, [this]() {
            const auto oldMask = T::mask();
            T::setMask(QRegion(this->rect()));
            T::setMask(oldMask);
            });
    }

    // if resizeable is set to false, then the window can not be resized by mouse
    // but still can be resized programtically
    void setResizeable(bool resizeable = true)
    {
        m_bResizeable = resizeable;
        HWND hwnd = reinterpret_cast<HWND>(this->effectiveWinId());
        if (hwnd == nullptr) {
            return;
        }

        // this will get titlebar/thick frame/Aero back, which is exactly what we want
        // we will get rid of titlebar and thick frame again in nativeEvent() later
        if (m_bResizeable) {
            const DWORD style = ::GetWindowLong(hwnd, GWL_STYLE);
            // WS_CAPTION: û���������Իᵼ���� VS �³��ִ��ڱ߿�����(���ڼ���״̬�л�ʱ), ���������Իᵼ�����ʱ���ݳ�����Ļ, ��˱�����������, ���ʱ�ߴ���������
            // WS_MAXIMIZEBOX: �������������֧�ִ����϶�����Ļ��Ե�Ŵ�Ч��
            // WS_SYSMENU: ������������, ����win7�³���ϵͳ�������С���͹رհ�ť
            // WS_THICKFRAME: �������������ʹ���ڸ���ϵͳ��ӰЧ��
            ::SetWindowLong(hwnd, GWL_STYLE, (style & ~WS_SYSMENU) | WS_MAXIMIZEBOX | WS_CAPTION | WS_THICKFRAME);
        }
        else {
            const DWORD style = ::GetWindowLong(hwnd, GWL_STYLE);
            // WS_MAXIMIZEBOX: ���������Ŵ���ʱ, ���ô�����
            ::SetWindowLong(hwnd, GWL_STYLE, (style & ~WS_SYSMENU & ~WS_MAXIMIZEBOX) | WS_CAPTION | WS_THICKFRAME);
        }

        // we better left 1 piexl width of border untouch, so OS can draw nice shadow around it
        const MARGINS shadow = { 1, 1, 1, 1 };
        DwmExtendFrameIntoClientArea(hwnd, &shadow);
    }

    bool isResizeable() const
    {
        return m_bResizeable;
    }

    // set border width, inside this aera, window can be resized by mouse
    void setResizeableAreaWidth(int width = 5)
    {
        if (1 > width)
            width = 1;
        m_borderWidth = width;
    }

protected:
    //set a widget which will be treat as SYSTEM titlebar
    void setTitleBar(QWidget* titlebar, bool autoIgnore = true)
    {
        m_titlebar = titlebar;

        if (titlebar != nullptr && autoIgnore) {
            auto children = titlebar->findChildren<QLabel*>();
            for (const auto& child : children) {
                addIgnoreWidget(child);
            }
        }
    }

    //generally, we can add widget say "label1" on titlebar, and it will cover the titlebar under it
    //as a result, we can not drag and move the MainWindow with this "label1" again
    //we can fix this by add "label1" to a ignorelist, just call addIgnoreWidget(label1)
    void addIgnoreWidget(QWidget* widget)
    {
        m_whiteList.insert(widget);
    }

#if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
    bool nativeEvent(const QByteArray& eventType, void* message, qintptr* result) override
#else
    bool nativeEvent(const QByteArray& eventType, void* message, long* result) override
#endif
    {
        if (!T::isWindow()) {
            return T::nativeEvent(eventType, message, result);
        }

        // Workaround for known bug -> check Qt forum : https://forum.qt.io/topic/93141/qtablewidget-itemselectionchanged/13
#if (QT_VERSION == QT_VERSION_CHECK(5, 11, 1))
        MSG* msg = *static_cast<MSG**>(message);
#else
        MSG* msg = static_cast<MSG*>(message);
#endif
        switch (msg->message) {
        case WM_NCCALCSIZE: {
            // this kills the window frame and title bar we added with WS_THICKFRAME and WS_CAPTION
            NCCALCSIZE_PARAMS* sz = reinterpret_cast<NCCALCSIZE_PARAMS*>(msg->lParam);
            if (!::IsZoomed(msg->hwnd)) {
                // sz->rgrc[0] ��ֵ�����ԭ���Ĳ�ͬ, ����������/�ϱ߿����Ŵ���ʱ, �ᵼ����/�²���ֿհ����� (���ƶ���)
                // �����±߿�ʧȥ1���ض��Ӿ�Ӱ����С, ��˵ײ�����1����
                sz->rgrc[0].bottom += 1;
            }
            else {
                // flags ���� 0x8000 ʱ, ��ζ�ſ��ܳ���������, ǿ��ˢ�´��ڱ������, ΢����ĵ����Ҳ������ֵ�Ķ���, ���о�
                if (sz->lppos->flags & 0x8000 && IsWindowVisible(msg->hwnd)) {
                    m_forceUpdateTimer.start(); // ����ǿ��ˢ�¶�ʱ��
                }
                else {
                    m_forceUpdateTimer.stop(); // ������������ʾ, û�б�Ҫ��ִ��ǿ��ˢ��
                    // �������ʱ���ݳ�����Ļ����
                    auto monitor = MonitorFromWindow(msg->hwnd, MONITOR_DEFAULTTONEAREST);
                    MONITORINFO info;
                    info.cbSize = sizeof(MONITORINFO);
                    if (GetMonitorInfo(monitor, &info)) {
                        const auto workRect = info.rcWork;
                        sz->rgrc[0].left = qMax(sz->rgrc[0].left, long(workRect.left));
                        sz->rgrc[0].top = qMax(sz->rgrc[0].top, long(workRect.top));
                        sz->rgrc[0].right = qMin(sz->rgrc[0].right, long(workRect.right));
                        sz->rgrc[0].bottom = qMin(sz->rgrc[0].bottom, long(workRect.bottom));
                    }
                }
            }

            *result = 0;
            return true;
        }
        case WM_NCHITTEST: {
            *result = 0;

            const LONG border_width = m_borderWidth;
            RECT winrect{};
            GetWindowRect(reinterpret_cast<HWND>(this->winId()), &winrect);

            const long x = GET_X_LPARAM(msg->lParam);
            const long y = GET_Y_LPARAM(msg->lParam);

            if (m_bResizeable && !IsZoomed(msg->hwnd)) {
                const bool resizeWidth = T::minimumWidth() != T::maximumWidth();
                const bool resizeHeight = T::minimumHeight() != T::maximumHeight();

                if (resizeWidth) {
                    // left border
                    if (x >= winrect.left && x < winrect.left + border_width) {
                        *result = HTLEFT;
                    }
                    // right border
                    if (x < winrect.right && x >= winrect.right - border_width) {
                        *result = HTRIGHT;
                    }
                }
                if (resizeHeight) {
                    // bottom border
                    if (y < winrect.bottom && y >= winrect.bottom - border_width) {
                        *result = HTBOTTOM;
                    }
                    // top border
                    if (y >= winrect.top && y < winrect.top + border_width) {
                        *result = HTTOP;
                    }
                }
                if (resizeWidth && resizeHeight) {
                    // bottom left corner
                    if (x >= winrect.left && x < winrect.left + border_width && y < winrect.bottom && y >= winrect.bottom - border_width) {
                        *result = HTBOTTOMLEFT;
                    }
                    // bottom right corner
                    if (x < winrect.right && x >= winrect.right - border_width && y < winrect.bottom && y >= winrect.bottom - border_width) {
                        *result = HTBOTTOMRIGHT;
                    }
                    // top left corner
                    if (x >= winrect.left && x < winrect.left + border_width && y >= winrect.top && y < winrect.top + border_width) {
                        *result = HTTOPLEFT;
                    }
                    // top right corner
                    if (x < winrect.right && x >= winrect.right - border_width && y >= winrect.top && y < winrect.top + border_width) {
                        *result = HTTOPRIGHT;
                    }
                }
            }
            if (0 != *result)
                return true;

            *result = DefWindowProc(msg->hwnd, msg->message, msg->wParam, msg->lParam);
            const static QSet<long> sizeBorders = { HTLEFT, HTRIGHT, HTTOP, HTBOTTOM, HTBOTTOMLEFT, HTBOTTOMRIGHT, HTTOPLEFT, HTTOPRIGHT };
            if (IsZoomed(msg->hwnd) && sizeBorders.contains(*result)) {
                *result = HTNOWHERE;
            }

            //*result still equals 0, that means the cursor locate OUTSIDE the frame area
            // but it may locate in titlebar area
            if (!m_titlebar)
                return true;

            // support highdpi
            const double dpr = this->devicePixelRatioF();
            const QPoint pos = m_titlebar->mapFromGlobal(QPoint(x / dpr, y / dpr));

            if (!m_titlebar->rect().contains(pos))
                return true;
            QWidget* child = m_titlebar->childAt(pos);
            if (!child) {
                *result = HTCAPTION;
                return true;
            }
            else {
                if (m_whiteList.contains(child)) {
                    *result = HTCAPTION;
                    return true;
                }
            }

            return true;
        } // end case WM_NCHITTEST
        case WM_WINDOWPOSCHANGING: {
            // Tell Windows to discard the entire contents of the client area, as re-using
            // parts of the client area would lead to jitter during resize.
            auto* windowPos = reinterpret_cast<WINDOWPOS*>(msg->lParam);
            windowPos->flags |= SWP_NOCOPYBITS;
            break;
        }
        default:
            break;
        }
        return T::nativeEvent(eventType, message, result);
    }

    bool event(QEvent* event) override
    {
        if (!T::isWindow()) {
            return T::event(event);
        }

        switch (event->type()) {
        case QEvent::WindowStateChange: {
            // �ڴ������ʱ, �����������ק��������ԭ����, ������Ȼ�������������, ��ʱ�ٽ��д��ڻ�ԭʱ(����˫��������, showNormal()�ȷ�ʽ), ����������һ��������Ļ֮��
            // ��������������û��ʹ���ޱ߿����Զ��ᷢ��, Ӧ���� Qt ����һ�� Bug (���Գ��� Qt 5.15/Qt 6.3 + Win10)
            // �˴�������Ϊ����
            if (T::windowState() == Qt::WindowNoState) {
                const auto workRect = qApp->primaryScreen()->availableVirtualGeometry();
                // ����ط�����ֱ���� pos() ����, ��Ϊ Qt �ĳߴ��λ����ؽӿ������ӳټ�֡, ��ʱ�õ��� pos() ��Ȼ�����ʱ��λ��
                RECT rect;
                GetWindowRect(reinterpret_cast<HWND>(this->winId()), &rect);

                if (rect.top < workRect.top()) {
                    T::move(rect.left, workRect.top());
                }
            }
            break;
        }
        case QEvent::WinIdChange:
            setResizeable(m_bResizeable);
            break;
#if 1//(QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
        case QEvent::ScreenChangeInternal: {
            // ͨ������Maskǿ�ƴ�������, ����������קʱ�Ĵ�λ����, ͬʱ�ᵼ��ʧȥ������Ӱ
            const auto oldMask = T::mask();
            T::setMask(QRegion(this->rect()));
            T::setMask(oldMask);

            // �������ô�������, �Ѵ�����Ӱ������
            setResizeable(m_bResizeable);
            break;
        }
#endif
        default:
            break;
        }
        return T::event(event);
    }

private:
    QPointer<QWidget> m_titlebar;
    QSet<QWidget*> m_whiteList;
    int m_borderWidth{ 5 };
    bool m_bResizeable{ true };
    QTimer m_forceUpdateTimer;
};

#endif // NATIVEWINDOWTEMPLATE_H
