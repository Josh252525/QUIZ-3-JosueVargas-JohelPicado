# QUIZ-3---AN-LISIS-DE-ALGORITMOS

# Análisis empírico de algoritmos.

#1. Busqueda Binaria.

<img width="1600" height="962" alt="image" src="https://github.com/user-attachments/assets/87cf33f2-2e53-4bdb-905c-23a50ec4c188" />


Esta gráfica presenta el tiempo de ejecución de la busqueda binaria con diferentes tamaños de arreglos. Como se puede observar, el tiempo de ejecución aumenta lentamente conforme crece el tamaño del arreglo, lo cual cumple con el principio del O(logn).

1.2: Tiempo de ejecución vs log₂(n)

<img width="1600" height="969" alt="image" src="https://github.com/user-attachments/assets/613e3f29-1fee-4b4c-bf90-edaf1e824cc6" />

Al representar el tiempo de ejecución en función de log₂(n) se obtiene una relación aproximadamente lineal. Como se puede ver, el ajuste lineal llega a ser 0,9957, muy cercano a 1, por lo que se respalda experimentalmente el comportamiento O(logn).

#2. Mergesort.

<img width="1600" height="961" alt="image" src="https://github.com/user-attachments/assets/7ba5eae5-27a3-424b-82e7-d8b3acbf2eef" />

La gráfica muestra que el tiempo promedio de la ejecución de Mergesort, de igual forma, varía para diferentes tamaños de arreglos. Se observa un incremento considerable del tiepmo de ejecución a medida que aumenta tamaño del arreglo. Si bien empieza más "ameno" que Busqueda Binaria, empieza a subir de forma exponencial a partir de 10^5 en el tamaño del arreglo.

<img width="1600" height="968" alt="image" src="https://github.com/user-attachments/assets/808e27c5-b033-4cab-8f51-fc8fc0e1d68a" />

Al representar el tiempo de ejecución en función de nlog₂(n), se observa una relación aproximádamente lineal, de hecho casi perfectamente lineal. El ajuste lineal tiene un coeficiente de determinación aún más alto que el de busqueda binaria, llegando a 0,999966, aún más cercano a 1. Esto respalda experimentalmente el comportamiento que posee de O(nlogn).

#3. Comparación del crecimiento.

<img width="1600" height="962" alt="image" src="https://github.com/user-attachments/assets/a6c0365c-c603-43ef-b179-bebff62107db" />

Esta gráfica compara el cociente T(2n)/T(n) de ambos algoritmos con sus respectivos valores teóricos. Para la busqueda binaria, el cociente tiende a 1, como se ve. Mergesort, por su lado, tinede principalmente a 2, con un pico al inicio. Los resultados experimentales, en general, presentan un comportamiento consistente con sus predicciones por medio de los modelos teóricos. 



