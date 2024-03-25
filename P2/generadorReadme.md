# Instrucciones de uso del generador

Puede ejecutarlo con:
```bash
./generador archivo
./generador archivo maxk
./generador archivo min max
./generador archivo min max maxk
```

Donde:
- `maxk` es el máximo valor de `k` en la expresión $n = 2^k$ para el segundo problema. Se establece por defecto a `30` (el valor mínimo de `k` es `0`).
- `min` es el valor mínimo que se tendrá en cuenta para generar números aleatorios (por defecto, `-1000000`).
- `max` es el valor máximo que se tendrá en cuenta para generar números aleatorios (por defecto, `1000000`).

## Si generamos instancias para el ejercicio 1:
- Se puede elegir entre un número fijo de números aleatorios o un número aleatorio (entre 0 y `max`). En dicho caso, los números generados estarán en `[min, max]`. El formato del archivo generado será:
```txt
n
random random random random random random random random random random random random random random random random random random random random random random random random random random random random random random random random random random random random random random random random random random random random random random random random random random ...
```
## Si generamos instancias para el ejercicio 2:
- Se generará un número aleatorio $k \in [0, maxk]$ y tendremos $n = 2^k$. Se generará un punto aleatorio con ambas coordenadas en `[0, n-1]`. El formato del archivo será:
```txt
n x y
```
## Si generamos instancias para el ejercicio 3:
- Se puede elegir entre un número fijo de puntos aleatorios o un número aleatorio (entre 0 y `max/2`). En dicho caso, los puntos generados estarán en `[min, max]`. No habrá puntos repetidos. El formato del archivo generado será:
```txt
n
x y
x y
x y
.
.
.
```
