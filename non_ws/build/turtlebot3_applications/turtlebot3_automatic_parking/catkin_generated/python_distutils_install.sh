#!/bin/sh

if [ -n "$DESTDIR" ] ; then
    case $DESTDIR in
        /*) # ok
            ;;
        *)
            /bin/echo "DESTDIR argument must be absolute... "
            /bin/echo "otherwise python's distutils will bork things."
            exit 1
    esac
fi

echo_and_run() { echo "+ $@" ; "$@" ; }

echo_and_run cd "/home/pirat/robocup-navigation/non_ws/src/turtlebot3_applications/turtlebot3_automatic_parking"

# ensure that Python install destination exists
echo_and_run mkdir -p "$DESTDIR/home/pirat/robocup-navigation/non_ws/install/lib/python2.7/dist-packages"

# Note that PYTHONPATH is pulled from the environment to support installing
# into one location when some dependencies were installed in another
# location, #123.
echo_and_run /usr/bin/env \
    PYTHONPATH="/home/pirat/robocup-navigation/non_ws/install/lib/python2.7/dist-packages:/home/pirat/robocup-navigation/non_ws/build/lib/python2.7/dist-packages:$PYTHONPATH" \
    CATKIN_BINARY_DIR="/home/pirat/robocup-navigation/non_ws/build" \
    "/usr/bin/python2" \
    "/home/pirat/robocup-navigation/non_ws/src/turtlebot3_applications/turtlebot3_automatic_parking/setup.py" \
     \
    build --build-base "/home/pirat/robocup-navigation/non_ws/build/turtlebot3_applications/turtlebot3_automatic_parking" \
    install \
    --root="${DESTDIR-/}" \
    --install-layout=deb --prefix="/home/pirat/robocup-navigation/non_ws/install" --install-scripts="/home/pirat/robocup-navigation/non_ws/install/bin"
