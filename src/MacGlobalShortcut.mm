#include "openCap/MacGlobalShortcut.h"

#ifdef Q_OS_MACOS

#import <Cocoa/Cocoa.h>
#import <Carbon/Carbon.h>
#include <QDebug>

// 静态实例指针
MacGlobalShortcut* MacGlobalShortcut::s_instance = nullptr;

// 热键ID
static const int HOTKEY_ID = 1;

MacGlobalShortcut::MacGlobalShortcut(QObject *parent)
    : QObject(parent)
    , m_eventHandler(nullptr)
    , m_hotKeyRef(nullptr)
    , m_registered(false)
{
    s_instance = this;
}

MacGlobalShortcut::~MacGlobalShortcut()
{
    unregisterShortcut();
    s_instance = nullptr;
}

bool MacGlobalShortcut::registerShortcut(const QKeySequence &shortcut)
{
    if (m_registered) {
        unregisterShortcut();
    }

    // 解析快捷键序列 (Meta+Shift+A)
    if (shortcut.count() != 1) {
        qWarning() << "只支持单一快捷键组合";
        return false;
    }

    QKeyCombination keyCombination = shortcut[0];
    int key = keyCombination.toCombined();
    
    // 提取修饰键
    int qtModifiers = (key & Qt::KeyboardModifierMask);
    int qtKey = (key & ~Qt::KeyboardModifierMask);
    
    // 转换为 Carbon 修饰键
    UInt32 carbonModifiers = 0;
    if (qtModifiers & Qt::MetaModifier) {
        carbonModifiers |= cmdKey;  // Command 键
    }
    if (qtModifiers & Qt::ShiftModifier) {
        carbonModifiers |= shiftKey; // Shift 键
    }
    if (qtModifiers & Qt::ControlModifier) {
        carbonModifiers |= controlKey; // Control 键
    }
    if (qtModifiers & Qt::AltModifier) {
        carbonModifiers |= optionKey; // Alt/Option 键
    }
    
    // 转换按键码
    UInt32 carbonKeyCode = 0;
    switch (qtKey) {
        case Qt::Key_A: carbonKeyCode = kVK_ANSI_A; break;
        case Qt::Key_B: carbonKeyCode = kVK_ANSI_B; break;
        case Qt::Key_C: carbonKeyCode = kVK_ANSI_C; break;
        case Qt::Key_D: carbonKeyCode = kVK_ANSI_D; break;
        case Qt::Key_E: carbonKeyCode = kVK_ANSI_E; break;
        case Qt::Key_F: carbonKeyCode = kVK_ANSI_F; break;
        case Qt::Key_G: carbonKeyCode = kVK_ANSI_G; break;
        case Qt::Key_H: carbonKeyCode = kVK_ANSI_H; break;
        case Qt::Key_I: carbonKeyCode = kVK_ANSI_I; break;
        case Qt::Key_J: carbonKeyCode = kVK_ANSI_J; break;
        case Qt::Key_K: carbonKeyCode = kVK_ANSI_K; break;
        case Qt::Key_L: carbonKeyCode = kVK_ANSI_L; break;
        case Qt::Key_M: carbonKeyCode = kVK_ANSI_M; break;
        case Qt::Key_N: carbonKeyCode = kVK_ANSI_N; break;
        case Qt::Key_O: carbonKeyCode = kVK_ANSI_O; break;
        case Qt::Key_P: carbonKeyCode = kVK_ANSI_P; break;
        case Qt::Key_Q: carbonKeyCode = kVK_ANSI_Q; break;
        case Qt::Key_R: carbonKeyCode = kVK_ANSI_R; break;
        case Qt::Key_S: carbonKeyCode = kVK_ANSI_S; break;
        case Qt::Key_T: carbonKeyCode = kVK_ANSI_T; break;
        case Qt::Key_U: carbonKeyCode = kVK_ANSI_U; break;
        case Qt::Key_V: carbonKeyCode = kVK_ANSI_V; break;
        case Qt::Key_W: carbonKeyCode = kVK_ANSI_W; break;
        case Qt::Key_X: carbonKeyCode = kVK_ANSI_X; break;
        case Qt::Key_Y: carbonKeyCode = kVK_ANSI_Y; break;
        case Qt::Key_Z: carbonKeyCode = kVK_ANSI_Z; break;
        default:
            qWarning() << "不支持的按键:" << qtKey;
            return false;
    }

    qDebug() << "注册全局快捷键: 修饰键=" << carbonModifiers << "按键码=" << carbonKeyCode;

    // 安装事件处理器
    EventTypeSpec eventType;
    eventType.eventClass = kEventClassKeyboard;
    eventType.eventKind = kEventHotKeyPressed;
    
    EventHandlerUPP handlerUPP = NewEventHandlerUPP((EventHandlerProcPtr)hotKeyHandler);
    OSStatus status = InstallApplicationEventHandler(
        handlerUPP,
        1,
        &eventType,
        this,
        (EventHandlerRef*)&m_eventHandler
    );
    
    if (status != noErr) {
        qWarning() << "安装事件处理器失败:" << status;
        return false;
    }

    // 注册热键
    EventHotKeyID hotKeyID;
    hotKeyID.signature = 'jiet'; // 四字符代码
    hotKeyID.id = HOTKEY_ID;
    
    EventHotKeyRef* hotKeyRefPtr = (EventHotKeyRef*)&m_hotKeyRef;
    status = RegisterEventHotKey(
        carbonKeyCode,
        carbonModifiers,
        hotKeyID,
        GetApplicationEventTarget(),
        0,
        hotKeyRefPtr
    );
    
    if (status != noErr) {
        qWarning() << "注册热键失败:" << status;
        RemoveEventHandler((EventHandlerRef)m_eventHandler);
        m_eventHandler = nullptr;
        return false;
    }

    m_registered = true;
    qDebug() << "全局快捷键注册成功: Command+Shift+A";
    return true;
}

void MacGlobalShortcut::unregisterShortcut()
{
    if (m_registered) {
        // 注销热键
        if (m_hotKeyRef) {
            UnregisterEventHotKey((EventHotKeyRef)m_hotKeyRef);
            m_hotKeyRef = nullptr;
        }
        
        // 移除事件处理器
        if (m_eventHandler) {
            RemoveEventHandler((EventHandlerRef)m_eventHandler);
            m_eventHandler = nullptr;
        }
        
        m_registered = false;
        qDebug() << "全局快捷键已注销";
    }
}

// Carbon 事件处理回调函数
int MacGlobalShortcut::hotKeyHandler(void *nextHandler, void *theEvent, void *userData)
{
    Q_UNUSED(nextHandler)
    Q_UNUSED(userData)
    
    EventRef event = (EventRef)theEvent;
    EventHotKeyID hotKeyID;
    GetEventParameter(event, kEventParamDirectObject, typeEventHotKeyID, 
                     NULL, sizeof(hotKeyID), NULL, &hotKeyID);
    
    if (hotKeyID.signature == 'jiet' && hotKeyID.id == HOTKEY_ID) {
        qDebug() << "全局快捷键被触发";
        
        if (s_instance) {
            // 使用 QMetaObject::invokeMethod 确保信号在主线程中发射
            QMetaObject::invokeMethod(s_instance, "activated", Qt::QueuedConnection);
        }
        
        return 0; // noErr
    }
    
    return -1; // eventNotHandledErr
}

#endif // Q_OS_MACOS 