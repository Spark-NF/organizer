#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTemporaryDir>
#include <catch.h>
#include "plugin-registry.h"
#include "processes/plugin-process.h"


static bool writeManifest(const QString &pluginDir, const QJsonObject &manifest)
{
	QDir().mkpath(pluginDir);
	QFile f(pluginDir + "/plugin.json");
	if (!f.open(QFile::WriteOnly)) return false;
	f.write(QJsonDocument(manifest).toJson());
	return true;
}


TEST_CASE("PluginRegistry")
{
	PluginRegistry::instance().reset();

	QTemporaryDir tmp;
	QJsonObject manifest {
		{"name", "test"},
		{"cmd", "echo"},
	};

	SECTION("Load loader type")
	{
		manifest["provides"] = QJsonObject{{"loaders", QJsonArray{"test_loader"}}};
		REQUIRE(writeManifest(tmp.filePath("myplugin"), manifest));

		PluginRegistry::instance().initialize({tmp.path()});

		REQUIRE(PluginRegistry::instance().loaderFor("test_loader") != nullptr);
		REQUIRE(PluginRegistry::instance().loaderFor("nonexistent") == nullptr);
	}

	SECTION("Load action type")
	{
		manifest["provides"] = QJsonObject{{"actions", QJsonArray{"test_action"}}};
		REQUIRE(writeManifest(tmp.filePath("myplugin"), manifest));

		PluginRegistry::instance().initialize({tmp.path()});

		REQUIRE(PluginRegistry::instance().actionFor("test_action") != nullptr);
		REQUIRE(PluginRegistry::instance().actionFor("nonexistent") == nullptr);
	}

	SECTION("Processes are shared shared across keys")
	{
		manifest["provides"] = QJsonObject{
			{"loaders", QJsonArray{"key_a", "key_b"}},
			{"actions", QJsonArray{"act_a"}},
		};
		REQUIRE(writeManifest(tmp.filePath("myplugin"), manifest));

		PluginRegistry::instance().initialize({tmp.path()});

		const auto plugin = PluginRegistry::instance().loaderFor("key_a");
		REQUIRE(plugin != nullptr);
		REQUIRE(PluginRegistry::instance().loaderFor("key_b") == plugin);
		REQUIRE(PluginRegistry::instance().actionFor("act_a") == plugin);
	}

	SECTION("collision")
	{
		manifest["provides"] = QJsonObject{{"loaders", QJsonArray{"shared_key"}}};

		manifest["name"] = "first";
		REQUIRE(writeManifest(tmp.filePath("first"), manifest));
		manifest["name"] = "second";
		REQUIRE(writeManifest(tmp.filePath("second"), manifest));

		PluginRegistry::instance().initialize({tmp.path()});

		const auto plugin = PluginRegistry::instance().loaderFor("shared_key");
		REQUIRE(plugin != nullptr);
		REQUIRE(plugin->name() == "first");
	}

	SECTION("Missing required field")
	{
		manifest.remove("cmd");
		REQUIRE(writeManifest(tmp.filePath("myplugin"), manifest));

		PluginRegistry::instance().initialize({tmp.path()});

		REQUIRE(PluginRegistry::instance().loaderFor("test_loader") == nullptr);
	}

	SECTION("Empty path is skipped")
	{
		PluginRegistry::instance().initialize({""});
		REQUIRE(PluginRegistry::instance().loaderFor("anything") == nullptr);
	}

	SECTION("Reset")
	{
		manifest["provides"] = QJsonObject{{"loaders", QJsonArray{"test_loader"}}};
		REQUIRE(writeManifest(tmp.filePath("myplugin"), manifest));

		PluginRegistry::instance().initialize({tmp.path()});
		REQUIRE(PluginRegistry::instance().loaderFor("test_loader") != nullptr);

		PluginRegistry::instance().reset();
		REQUIRE(PluginRegistry::instance().loaderFor("test_loader") == nullptr);
	}
}
