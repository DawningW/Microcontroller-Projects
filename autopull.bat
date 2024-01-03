@echo off
echo Start pulling down sub projects...
git subtree pull --prefix=lightcube lightcube master
git subtree pull --prefix=minichatserver minichatserver master
git subtree pull --prefix=Application/qt qt master
git subtree pull --prefix=Application/android android master
git subtree pull --prefix=Application/ios ios master
git subtree pull --prefix=Application/web web master
echo Finished!
pause