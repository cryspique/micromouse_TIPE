eval cc micromouse.c $(pkg-config --libs --cflags raylib) -o micromouse.out
eval cc trajectory.c $(pkg-config --libs --cflags raylib) -o trajectory.out
