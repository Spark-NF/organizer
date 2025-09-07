#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <QStringList>
#include <functional>

#define DEFAULT_INTERVAL 500
#define DEFAULT_TIMEOUT 3000


class QAction;
class QMainWindow;


template <typename T>
void waitForWindow(std::function<void(T*)> callback, int interval = DEFAULT_INTERVAL, int timeout = DEFAULT_TIMEOUT);

template <typename T>
void openWindow(std::function<void()> callback);

template <typename T>
void openAndWaitForWindow(std::function<void(T*)> callback, int interval = DEFAULT_INTERVAL, int timeout = DEFAULT_TIMEOUT);

QAction *getMenuAction(const QMainWindow *window, QStringList path);


#include "test-utils.cxx"

#endif // TEST_UTILS_H
