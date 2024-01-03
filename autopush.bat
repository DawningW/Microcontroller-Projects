@echo off
echo Start pushing up sub projects...
git subtree push --prefix=lightcube lightcube master
git subtree push --prefix=minichatserver minichatserver master
git subtree push --prefix=Application/qt qt master
git subtree push --prefix=Application/android android master
git subtree push --prefix=Application/ios ios master
git subtree push --prefix=Application/web web master
echo Finished!
pause