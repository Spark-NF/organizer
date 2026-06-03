#ifndef TEMPLATE_STRING_H
#define TEMPLATE_STRING_H

#include <utility>
#include <QList>
#include <QString>
#include <QStringList>
#include <QVariantMap>


struct Placeholder
{
	int start;
	int end;
	QString key;
	QStringList fields;
	QList<std::pair<QString, QString>> filters;
	bool hasDefault = false;
};


class TemplateString
{
	public:
		TemplateString(const QString &tmpl);
		TemplateString(const char *tmpl);

		QString resolve(const QVariantMap &data, QString *error = nullptr) const;
		QList<std::pair<QString, QStringList>> requiredKeys() const;
		bool hasUnknownFilters() const;
		QString pattern() const;

	protected:
		static QVariant applyFilter(const QVariant &value, const std::pair<QString, QString> &pair);

	private:
		QString m_template;
		QList<Placeholder> m_placeholders;
		QList<std::pair<QString, QStringList>> m_requiredKeys;
};

#endif // TEMPLATE_STRING_H
