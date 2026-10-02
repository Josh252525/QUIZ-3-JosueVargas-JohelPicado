# Análisis académico del benchmark

## Metodología

Se implementaron desde cero en C++17 la búsqueda binaria y Mergesort. La entrada base para cada tamaño es una permutación aleatoria de 0,...,n-1, generada con `std::mt19937_64` y semilla fija 20261001. La copia ordenada utilizada por búsqueda binaria se preparó antes de la región cronometrada. Para las mediciones de búsqueda se utilizaron 10 000 consultas aleatorias por repetición; las consultas están fuera del intervalo `[0,n-1]`, por lo que corresponden al caso desfavorable de la búsqueda binaria y fuerzan una exploración hasta la profundidad completa. Para Mergesort se ordenó una copia de la permutación aleatoria; la copia y la verificación posterior no se incluyeron en el tiempo medido.

Se ejecutaron 15 repeticiones por tamaño, usando `std::chrono::steady_clock`. Se registraron promedio, mediana y desviación estándar. Los tamaños fueron 1 000, 2 000, 5 000, 10 000, 20 000, 50 000, 100 000, 200 000, 500 000 y 1 000 000.

## Búsqueda binaria

Se ajustó el modelo:

T(n) = a·log₂(n) + b

sobre las medianas. El ajuste obtuvo:

a = 1.073405 ns
b = 0.152702 ns
R² = 0.995703

El tiempo mediano pasó de 10.5590 ns/búsqueda para n=1,000 a 21.4925 ns/búsqueda para n=1,000,000. La relación T(n) frente a log₂(n) es fuertemente lineal en el rango observado. El R² no es una demostración de complejidad; indica qué tan bien una recta en función de log₂(n) describe los datos medidos.

## Mergesort

Se ajustó:

T(n) = a·(n log₂(n)) + b

sobre las medianas. Se obtuvo:

a = 0.0000041312 ms
b = 0.056131 ms
R² = 0.999966066

El ajuste presenta una relación prácticamente lineal entre el tiempo medido y n log₂(n) para el intervalo evaluado.

## Cociente T(2n)/T(n)

Resultados con medianas:

1,000 → 2,000: binaria 1.1025 (teoría 1.1003); Mergesort 2.1847 (teoría 2.2007)
5,000 → 10,000: binaria 1.0587 (teoría 1.0814); Mergesort 2.5058 (teoría 2.1628)
10,000 → 20,000: binaria 1.0742 (teoría 1.0753); Mergesort 2.2245 (teoría 2.1505)
50,000 → 100,000: binaria 1.0648 (teoría 1.0641); Mergesort 2.1075 (teoría 2.1281)
100,000 → 200,000: binaria 1.0635 (teoría 1.0602); Mergesort 2.1610 (teoría 2.1204)
500,000 → 1,000,000: binaria 1.0534 (teoría 1.0528); Mergesort 2.1006 (teoría 2.1056)

Para búsqueda binaria, el cociente teórico converge a 1. En las parejas de mayor tamaño el cociente experimental se acerca a ese valor. Para Mergesort, el cociente teórico converge a 2 y los datos experimentales se mantienen alrededor de 2, con pequeñas desviaciones atribuibles a factores de ejecución.

## Control de calidad y limitaciones

El benchmark evita medir preparación de datos, ordenamiento inicial del arreglo usado por búsqueda binaria, copia de la entrada para Mergesort, impresión y escritura del archivo. Se usa una semilla fija para reproducibilidad. También se impide que el compilador elimine las búsquedas mediante un acumulador `volatile`, y se verifica que Mergesort produzca una secuencia ordenada.

Los tiempos absolutos dependen del hardware, sistema operativo, compilador, optimización, cachés, administración de memoria y carga del sistema. En búsqueda binaria cada operación dura unas decenas de nanosegundos, por lo que la variación de microarquitectura y temporización tiene un peso relativo mayor que en Mergesort. La medición se repite y se agregan 10 000 consultas para reducir el ruido de una medición individual.

El hecho de que un ajuste tenga R² cercano a 1 no constituye, por sí solo, una demostración de O(log n) u O(n log n). El análisis teórico de los algoritmos proporciona la justificación matemática; el benchmark sirve como evidencia experimental de que los tiempos observados son compatibles con esas funciones de crecimiento.

## Conclusión

Dentro de los tamaños, hardware y condiciones del experimento, los resultados muestran un comportamiento compatible con O(log n) para búsqueda binaria y O(n log n) para Mergesort. La búsqueda binaria presenta una relación aproximadamente lineal cuando el tiempo se representa contra log₂(n), mientras que Mergesort presenta una relación prácticamente lineal cuando se representa contra n log₂(n). El análisis mediante T(2n)/T(n) refuerza esta interpretación: el crecimiento experimental se aproxima a 1 para búsqueda binaria y a 2 para Mergesort conforme n aumenta.

Esta evidencia experimental complementa, pero no sustituye, la demostración asintótica basada en el análisis de las operaciones y recurrencias de cada algoritmo.
