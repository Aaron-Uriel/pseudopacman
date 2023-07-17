# Pac-Man.
Implementación de un clon de Pac-Man en C utilizando Ncursesw.

## ¿Cómo compilo este proyecto?
Solo es necesario tener meson, un compilador de C y la libreriía Ncursesw instaladas.
Para compilarlo crea primero la la carpeta de compilación con:
```bash
meson setup build
```
Luego compilalo:
```bash
meson compile -C
```
Y ejecútalo (el ejecutable estará dentro de build):
```bash
./build/pacman
```

## Objetivos
- [ ] Factorización del código (hasta lo que está en 96440b1).
- [ ] Menú interactivo al principio del programa.
- [ ] Añadir salud (que Pac-Man pueda morir).
- [ ] Dinámica de puntaje (las cosas que se traga Pac-Man xd).
- [ ] Dinámica de dificultad.
