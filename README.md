# Productores-y-consumidores-con-semaforos

## Consumer busy waiting

Para adaptar el código a semáforos pensé en eliminar la espera activa y usar mecanismos de sincronización que controlaran el acceso al buffer. Definí tres semáforos: uno para los huecos vacíos (`espacios_vacios`), otro para los huecos ocupados (`espacios_ocupados`), y un semáforo tipo mutex para proteger la sección crítica (`mutex`) (no tengo seguro si haría falta ya que se protege con 2 semaforos los buffers (ponerlo no hace daño supongo)). Inicialicé los semáforos de forma que al principio el buffer estuviera vacío y los productores pudieran empezar a escribir.  

Funcionamiento: el productor espera a que haya espacio disponible con `sem_wait(&espacios_vacios)`, entra a la sección crítica con `sem_wait(&mutex)`, escribe en el buffer y libera los semáforos al salir. El consumidor hace lo mismo pero en sentido contrario: espera a que haya datos con `sem_wait(&espacios_ocupados)`, entra al buffer protegido, lee un valor y libera un hueco con `sem_post(&espacios_vacios)`. Puse el valor 10 en `sem_init(&espacios_vacios, 0, MAX_BUFFER)` porque representa el tamaño total del buffer, indicando que al inicio hay diez posiciones vacías disponibles para que el productor empiece a llenarlas.

## Consumer condvariable multi

En esta versión intenté pasar el código que usaba `pthread_cond_t` y `pthread_mutex_t` a semáforos. Aquí ya había varios productores trabajando a la vez, así que tuve que cuidar más el acceso a la sección crítica y la gestión del buffer compartido. Reemplacé las variables de condición `not_full` y `not_empty` por los semáforos `espacios_vacios` y `espacios_ocupados`, y el mutex por un semáforo binario. La lógica es parecida al caso anterior, pero con más hilos produciendo al mismo tiempo y un consumidor único que va leyendo todo lo que producen los demás.  

Me lié un poco al principio con la lógica del `pthread_cond`, porque aunque entendía cómo funcionaban las variables de condición y también los semáforos, no tenía claro cómo pasar de una cosa a la otra. Sabía que con `pthread_cond_wait` el hilo se quedaba bloqueado hasta que otro hacía un `pthread_cond_signal`, pero no veía claramente cómo trasladar eso a semáforos sin romper la sincronización. Al final entendí que esa misma lógica de espera y desbloqueo se podía hacer con `sem_wait()` y `sem_post()`, igual que había hecho al modificar el código anterior. Una vez que vi eso, pude sustituir las condiciones por semáforos sin necesidad de usar `pthread_cond_signal()` ni `pthread_cond_broadcast()`, y conseguí que los productores y el consumidor se coordinaran correctamente.

