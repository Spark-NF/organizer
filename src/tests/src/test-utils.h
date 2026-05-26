#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <QStringList>
#include <functional>

static constexpr int DefaultInterval = 500;
static constexpr int DefaultTimeout = 3000;


class QAction;
class QMainWindow;


template <typename T>
void waitForWindow(std::function<void(T*)> callback, int interval = DefaultInterval, int timeout = DefaultTimeout);

template <typename T>
void openWindow(std::function<void()> callback);

template <typename T>
void openAndWaitForWindow(std::function<void(T*)> callback, int interval = DefaultInterval, int timeout = DefaultTimeout);

QAction *getMenuAction(const QMainWindow *window, QStringList path);


#include "test-utils.cxx"

#endif // TEST_UTILS_H
