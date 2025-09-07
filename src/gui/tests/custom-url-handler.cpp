#include "custom-url-handler.h"


void CustomUrlHandler::handle(const QUrl &url)
{
	urls.append(url);
}