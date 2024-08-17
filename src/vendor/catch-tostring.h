#include <catch2/catch_tostring.hpp>
#include <QString>


namespace Catch
{
	template <>
	struct StringMaker<QString>
	{
		static std::string convert(QString const &value)
		{
			return '"' + value.toStdString() + '"';
		}
	};

	template <>
	struct StringMaker<QLatin1String>
	{
		static std::string convert(QLatin1String const &value)
		{
			return StringMaker<QString>::convert(value);
		}
	};

	template <>
	struct StringMaker<QChar>
	{
		static std::string convert(QChar const &value)
		{
			return "'" + std::string(1, value.toLatin1()) + "'";
		}
	};

	template <>
	struct StringMaker<QByteArray>
	{
		static std::string convert(QByteArray const &value)
		{
			return StringMaker<QString>::convert(value);
		}
	};
}
