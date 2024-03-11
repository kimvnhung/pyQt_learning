# pyQt_learning

# How to Generate Python Module from C++ on Window

## Install Pyside 6 , shiboken 6 and shiboken-generator6

    - pip install --index-url=https://download.qt.io/official_releases/QtForPython/ --trusted-host download.qt.io shiboken6 pyside6 shiboken6_generator

## Generate lib and python module

    from the folder which contains the CMakeLists.txt
    - mkdir build
    - cmake ..
    - (optional) cmake -H.. -B. -G "Visual Studio 17 2022"
    - cmake --build . --target install --config Release

## Run to Test imported

    - python main.py
