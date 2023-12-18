set_xmakever("2.8.5")
-- 设置工程名
set_project("nozzle design")

add_rules("mode.debug", "mode.release")
if is_mode("release") then
    set_symbols("hidden")
    set_optimize("faster") -- set optimization: none, fast, faster, fastest, smallest, aggressive
    set_strip("all")
elseif is_mode("debug") then
    set_symbols("debug")
    set_optimize("none")
    add_defines("NDEBUG")
end
set_fpmodels("precise")

set_warnings("all") -- set warning: none, less, more, all, error
set_languages("cxx20")

add_requireconfs("*", {system = true, configs = {shared = true}})

target("hkutools")
    add_rules("qt.widgetapp")
    add_headerfiles("src/*.h")
    add_files("src/*.cpp")
    add_files("src/mainwindow.ui")
    add_files("resources/*.qrc")
    add_files("resources/*.rc")
    -- add files with Q_OBJECT meta (only for qt.moc)
    add_files("src/mainwindow.h")
    -- add_frameworks("QtGui")
    set_encodings("utf-8")
    set_installdir("D:/apps/study/HKUtools/")
target_end()
