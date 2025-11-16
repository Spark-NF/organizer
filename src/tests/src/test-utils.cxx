#include "test-utils.h"
#include <QApplication>
#include <QElapsedTimer>
#include <QSignalSpy>
#include <QTimer>
#include <QWidget>
#include <catch.h>


template <typename T>
void waitForWindow(std::function<void(T*)> callback, int interval, int timeout)
{
	auto *elapsed = new QElapsedTimer();
	auto *intervalTimer = new QTimer();

	QObject::connect(intervalTimer, &QTimer::timeout, [=]() {
		if (elapsed->elapsed() > timeout) {
			intervalTimer->deleteLater();
			delete elapsed;

			FAIL("Window not found");
			return;
		}

		auto *activeWindow = qApp->activeWindow();
		auto *castedWindow = qobject_cast<T*>(activeWindow);
		if (castedWindow != nullptr) {
			intervalTimer->deleteLater();
			delete elapsed;

			callback(castedWindow);
		}
	});

	elapsed->start();
	intervalTimer->start(interval);
}

template <typename T>
void openWindow(std::function<void()> callback)
{
	QTimer::singleShot(0, callback);

	T window;
	QSignalSpy spy(&window, &T::closed);
	window.show();
	spy.wait();
}

template <typename T>
void openAndWaitForWindow(std::function<void(T*)> callback, int interval, int timeout)
{
	openWindow<T>([&]() {
		waitForWindow<T>(callback, interval, timeout);
	});
}
