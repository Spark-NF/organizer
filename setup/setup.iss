; Application information
#define MyAppName "Organizer"
#define MyAppPublisher "Spark-NF"
#define MyAppURL "https://github.com/Spark-NF/Organizer"
#define MyAppExeName "Organizer.exe"

; Useful variables
#define BuildDir "..\build"
#define ResourcesDir "..\src\gui\resources"
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
DisableDirPage=no
ArchitecturesAllowed=x86 x64 ia64
ArchitecturesInstallIn64BitMode=x64 ia64

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "{#ResourcesDir}\images\icon.ico";      DestDir: "{app}"; Flags: ignoreversion
Source: "{#BuildDir}\Organizer.exe";            DestDir: "{app}"; Flags: ignoreversion
Source: "{#BuildDir}\Organizer-cli.exe";        DestDir: "{app}"; Flags: ignoreversion
Source: "{#QtBin}\D3Dcompiler_47.dll";          DestDir: "{app}"; Flags: ignoreversion
Source: "{#QtBin}\opengl32sw.dll";              DestDir: "{app}"; Flags: ignoreversion
Source: "{#QtBin}\Qt6Core.dll";                 DestDir: "{app}"; Flags: ignoreversion
Source: "{#QtBin}\Qt6Gui.dll";                  DestDir: "{app}"; Flags: ignoreversion
Source: "{#QtBin}\Qt6Multimedia.dll";           DestDir: "{app}"; Flags: ignoreversion
Source: "{#QtBin}\Qt6MultimediaWidgets.dll";    DestDir: "{app}"; Flags: ignoreversion
Source: "{#QtBin}\Qt6Network.dll";              DestDir: "{app}"; Flags: ignoreversion
Source: "{#QtBin}\Qt6Svg.dll";                  DestDir: "{app}"; Flags: ignoreversion
Source: "{#QtBin}\Qt6Widgets.dll";              DestDir: "{app}"; Flags: ignoreversion
Source: "{#QtPlugins}\imageformats\qgif.dll";   DestDir: "{app}\imageformats"; Flags: ignoreversion
Source: "{#QtPlugins}\imageformats\qicns.dll";  DestDir: "{app}\imageformats"; Flags: ignoreversion
Source: "{#QtPlugins}\imageformats\qico.dll";   DestDir: "{app}\imageformats"; Flags: ignoreversion
Source: "{#QtPlugins}\imageformats\qjpeg.dll";  DestDir: "{app}\imageformats"; Flags: ignoreversion
Source: "{#QtPlugins}\imageformats\qsvg.dll";   DestDir: "{app}\imageformats"; Flags: ignoreversion
Source: "{#QtPlugins}\imageformats\qtga.dll";   DestDir: "{app}\imageformats"; Flags: ignoreversion
Source: "{#QtPlugins}\imageformats\qtiff.dll";  DestDir: "{app}\imageformats"; Flags: ignoreversion
Source: "{#QtPlugins}\imageformats\qwbmp.dll";  DestDir: "{app}\imageformats"; Flags: ignoreversion
Source: "{#QtPlugins}\imageformats\qwebp.dll";  DestDir: "{app}\imageformats"; Flags: ignoreversion
Source: "{#QtPlugins}\multimedia\windowsmediaplugin.dll";  DestDir: "{app}\multimedia"; Flags: ignoreversion
Source: "{#QtPlugins}\platforms\qwindows.dll";  DestDir: "{app}\platforms"; Flags: ignoreversion
Source: "{#QtPlugins}\styles\qwindowsvistastyle.dll";   DestDir: "{app}\styles"; Flags: ignoreversion

[InstallDelete]
Type: files; Name: "{app}\libEGL.dll"
Type: files; Name: "{app}\libGLESv2.dll"
Type: files; Name: "{app}\Qt5Core.dll"
Type: files; Name: "{app}\Qt5Gui.dll"
Type: files; Name: "{app}\Qt5Multimedia.dll"
Type: files; Name: "{app}\Qt5MultimediaWidgets.dll"
Type: files; Name: "{app}\Qt5Network.dll"
Type: files; Name: "{app}\Qt5OpenGL.dll"
Type: files; Name: "{app}\Qt5Widgets.dll"
Type: files; Name: "{app}\imageformats\qdds.dll"

[Icons]
Name: "{commonprograms}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{commondesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent
