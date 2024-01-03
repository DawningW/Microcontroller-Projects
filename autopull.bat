@echo off
echo Start pulling down sub projects...
git subtree pull --prefix=lightcube lightcube master
git subtree pull --prefix=minichatserver minichatserver master
git subtree pull --prefix=DebugTool/qt qt master
git subtree pull --prefix=DebugTool/android android master
echo Finished!
pause
