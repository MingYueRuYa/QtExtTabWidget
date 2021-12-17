# 代码片段



## 修改windows窗口属性

```C++
bool HostInfo::event(QEvent *event)
{
#ifdef Q_OS_WIN32
	static bool class_amended = false;
	if (event->type() == QEvent::WinIdChange) {
		HWND hwnd = reinterpret_cast<HWND>(winId());
		if (class_amended == false) {
			class_amended = true;
			DWORD class_style = ::GetClassLong(hwnd, GCL_STYLE);
			class_style &= ~CS_DROPSHADOW;
			::SetClassLong(hwnd, GCL_STYLE, class_style);
		}
	}
#endif
	return QWidget::event(event);
}
```

