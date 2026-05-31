#ifndef OPERATION_LOGGER_H
#define OPERATION_LOGGER_H

#include <QFile>
#include <QString>
#include <QTextStream>


class OperationLogger
{
	public:
		explicit OperationLogger(const QString &path);

		static OperationLogger &instance();
		static QString defaultPath();

		void logSkipped(const QString &source);
		void logExecuted(const QString &source, const QString &ruleName, bool ok, const QString &destination, const QString &error = QString());

	protected:
		void appendEntry(const QString &source, const QString &ruleName, const QString &result, const QString &destination, const QString &error = QString());

	private:
		QFile m_file;
		QTextStream m_stream;
};

#endif // OPERATION_LOGGER_H
