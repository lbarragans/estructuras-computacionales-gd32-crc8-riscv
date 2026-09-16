# CRC-8 con FreeRTOS puro

La aplicacion esta escrita en C. FreeRTOS organiza el ejercicio mediante tres
tareas y dos colas:

```text
ProducerTask -> job_queue -> ProcessorTask -> result_queue -> ValidatorTask
```

- `ProducerTask` publica el vector estandar y el mensaje alterado.
- `ProcessorTask` calcula CRC-8/ATM.
- `ValidatorTask` verifica `0xF4`, comprueba la alteracion y muestra el CRC en
  PC13.

La implementacion utiliza `xTaskCreate`, `xQueueCreate`, `xQueueSend`,
`xQueueReceive` y `vTaskDelay`. No llama la rutina Assembly de la referencia y
no utiliza espera activa.

La integracion usa el kernel, port Nuclei/ECLIC, heap y tick del MSDK oficial
V1.0.3g. La validacion fisica final se realiza siguiendo `INTEGRACION.md`.
