; Application information
#define MyAppName "Organizer"
#define MyAppPublisher "Spark-NF"
#define MyAppURL "https://github.com/Spark-NF/Organizer"
#define MyAppExeName "Organizer.exe"

; Useful variables
#define BuildDir "..\build"
#define ResourcesDir "..\resources"
#define QtBin QtDir + "\bin"
#define QtPlugins QtDir + "\plugins"

[Setup]
AppId={{20A47FD9-5911-4DB7-9A45-66F6431C18FB}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={pf}\{#MyAppName}
DisableProgramGroupPage=yes
LicenseFile=..\LICENSE
OutputDir=.
OutputBaseFilename=Organizer_{#MyAppVersion}_{#PlatformName}
SetupIconFile={#ResourcesDir}\images\icon.ico
UninstallDisplayName="{#MyAppName} {#MyAppVersion}"
UninstallDisplayIcon={app}\icon.ico
WizardSmallImageFile={#ResourcesDir}\images\setup.bmp
Compression=lzma
SolidCompression=yes
DisableReadyPage=no
DisableReadyMemo=no

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "{#ResourcesDir}\images\icon.ico";         DestDir: "{app}"; Flags: ignoreversion
Source: "{#BuildDir}\Organizer.exe";            DestDir: "{app}"; Flags: ignoreversion
Source: "{#QtBin}\libEGL.dll";                  DestDir: "{app}"; Flags: ignoreversion
Source: "{#QtBin}\libGLESv2.dll";               DestDir: "{app}"; Flags: ignoreversion
Source: "{#QtBin}\Qt5Core.dll";                 DestDir: "{app}"; Flags: ignoreversion
Source: "{#QtBin}\Qt5Gui.dll";                  DestDir: "{app}"; Flags: ignoreversion
Source: "{#QtBin}\Qt5Multimedia.dll";           DestDir: "{app}"; Flags: ignoreversion
Source: "{#QtBin}\Qt5MultimediaWidgets.dll";    DestDir: "{app}"; Flags: ignoreversion
Source: "{#QtBin}\Qt5Network.dll";              DestDir: "{app}"; Flags: ignoreversion
Source: "{#QtBin}\Qt5OpenGL.dll";               DestDir: "{app}"; Flags: ignoreversion
Source: "{#QtBin}\Qt5Widgets.dll";              DestDir: "{app}"; Flags: ignoreversion
Source: "{#QtPlugins}\imageformats\qdds.dll";   DestDir: "{app}\imageformats"; Flags: ignoreversion skipifsourcedoesntexist
Source: "{#QtPlugins}\imageformats\qgif.dll";   DestDir: "{app}\imageformats"; Flags: ignoreversion
Source: "{#QtPlugins}\imageformats\qicns.dll";  DestDir: "{app}\imageformats"; Flags: ignoreversion
Source: "{#QtPlugins}\imageformats\qico.dll";   DestDir: "{app}\imageformats"; Flags: ignoreversion
Source: "{#QtPlugins}\imageformats\qjpeg.dll";  DestDir: "{app}\imageformats"; Flags: ignoreversion
Source: "{#QtPlugins}\imageformats\qsvg.dll";   DestDir: "{app}\imageformats"; Flags: ignoreversion
Source: "{#QtPlugins}\imageformats\qtga.dll";   DestDir: "{app}\imageformats"; Flags: ignoreversion
Source: "{#QtPlugins}\imageformats\qtiff.dll";  DestDir: "{app}\imageformats"; Flags: ignoreversion
Source: "{#QtPlugins}\imageformats\qwbmp.dll";  DestDir: "{app}\imageformats"; Flags: ignoreversion
Source: "{#QtPlugins}\imageformats\qwebp.dll";  DestDir: "{app}\imageformats"; Flags: ignoreversion
Source: "{#QtPlugins}\platforms\qminimal.dll";     DestDir: "{app}\platforms"; Flags: ignoreversion
Source: "{#QtPlugins}\platforms\qoffscreen.dll";   DestDir: "{app}\platforms"; Flags: ignoreversion
Source: "{#QtPlugins}\platforms\qwindows.dll";     DestDir: "{app}\platforms"; Flags: ignoreversion
Source: "{#QtPlugins}\styles\qwindowsvistastyle.dll";   DestDir: "{app}\styles"; Flags: ignoreversion

[Icons]
Name: "{commonprograms}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{commondesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent
