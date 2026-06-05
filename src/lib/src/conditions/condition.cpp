#include "condition.h"
#include "comparators/comparator.h"
#include "media.h"


bool Condition::matchAndCapture(const QVariant &data, const Comparator &comparator, Media &media, const QString &key)
{
	QVariantMap output;
	const bool matched = comparator.match(data, &output);
	if (matched && !output.isEmpty()) {
		QVariantMap allCaptures = media.data().value("captures").toMap();
		allCaptures.insert(key, output);
		media.data()["captures"] = allCaptures;
	}
	return matched;
}
