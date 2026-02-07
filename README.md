# V5U BALAM MK1 - Robot de Competencia VEX V5

Robot de competencia VEX V5 con base omnidireccional X-Drive y sistema de intake. Diseñado para competiciones universitarias VEX U.

## Características Principales

- **Base X-Drive Omnidireccional**: Movimiento en cualquier dirección sin necesidad de girar
- **Control de Precisión**: Sistema de normalización para mantener vectores de movimiento correctos
- **Sistema de Intake**: Mecanismo para recoger y expulsar objetos
- **Deadband Inteligente**: Elimina deriva por joysticks no centrados perfectamente

## Configuración de Hardware

### Motores de Tracción (X-Drive)

| Motor | Puerto | Ratio | Invertido | Descripción |
|-------|--------|-------|-----------|-------------|
| FrontLeft | PORT1 | 18:1 (Verde) | No | Frontal izquierdo |
| FrontRight | PORT2 | 18:1 (Verde) | **Sí** | Frontal derecho |
| BackLeft | PORT3 | 18:1 (Verde) | No | Trasero izquierdo |
| BackRight | PORT4 | 18:1 (Verde) | **Sí** | Trasero derecho |

### Otros Sistemas

| Sistema | Puerto | Ratio | Descripción |
|---------|--------|-------|-------------|
| Intake | PORT5 | 18:1 (Verde) | Sistema de recolección |

### Controlador

- **Controller1**: Control primario (VEX V5 Controller)

## Sistema de Control

### Control de Movimiento (X-Drive)

El robot utiliza un sistema X-Drive que permite movimiento omnidireccional completo:

| Control | Función | Rango |
|---------|---------|-------|
| **Joystick Izquierdo Vertical** (Axis3) | Avanzar/Retroceder | -100 a 100 |
| **Joystick Izquierdo Horizontal** (Axis4) | Strafe (Lateral) | -100 a 100 |
| **Joystick Derecho Horizontal** (Axis1) | Girar | -100 a 100 |

### Control de Intake

| Botón | Función | Velocidad |
|-------|---------|-----------|
| **L2** | Meter objetos (reversa) | 100% |
| **R2** | Sacar objetos (adelante) | 100% |

## Cinemática del X-Drive

El código implementa la cinemática correcta para un X-Drive con ruedas a 45°:

```cpp
frontLeftSpeed  = throttle + strafe + turn
frontRightSpeed = throttle - strafe - turn
backLeftSpeed   = throttle - strafe + turn
backRightSpeed  = throttle + strafe - turn
```

### Sistema de Normalización

Para evitar saturación de motores (valores > 100%), el código incluye un sistema de normalización que:

1. Calcula todas las velocidades según la cinemática
2. Encuentra el valor máximo absoluto
3. Si excede 100%, escala proporcionalmente todas las velocidades
4. **Resultado**: Mantiene el vector de movimiento correcto incluso con joysticks al máximo

**Ejemplo:**
```
Input: throttle=100, strafe=100, turn=100
Cálculo bruto: frontLeftSpeed = 300

Sin normalización: 300 → saturado a 100 (distorsión)
Con normalización: 300 → escalado a 100, resto proporcional (correcto)
```

## Estructura del Código

```
V5U_BALAM_MK1_COMP/
├── src/
│   └── main.cpp          # Código principal del robot
├── include/
│   └── vex.h             # Definiciones VEX
├── vex/                  # Archivos de configuración VEX
├── makefile              # Compilación del proyecto
└── README.md             # Este archivo
```

## Funciones Principales

### `pre_auton()`
Configuración antes de la competencia:
- Configura modo de frenado (`brake`) en todos los motores
- Resetea encoders de posición
- Se ejecuta una sola vez al encender

### `autonomous()`
Rutina autónoma de 15 segundos:
- Actualmente vacía (por implementar)
- Para agregar rutinas automáticas de competencia

### `usercontrol()`
Control del piloto (1:45 minutos):
- Loop de control a 50Hz (20ms)
- Implementa cinemática X-Drive completa
- Sistema de deadband (±5 unidades)
- Normalización de velocidades
- Control de intake

## Compilación y Despliegue

### Requisitos
- VEX V5 Brain
- VEXcode Pro V5
- Cable USB o conexión inalámbrica V5

### Compilar
```bash
make
```

### Desplegar al Robot
1. Conectar V5 Brain por USB o VEXnet
2. Abrir el archivo en VEXcode Pro V5
3. Compilar y descargar al cerebro

## Parámetros Técnicos

| Parámetro | Valor |
|-----------|-------|
| Deadband | ±5 unidades |
| Frecuencia de control | 50 Hz (20ms) |
| Modo de frenado | Brake |
| Velocidad intake | 100% |
| Ratio de motores | 18:1 (Torque) |

## Características Técnicas

### Ventajas del X-Drive
- Movimiento omnidireccional completo
- Desplazamiento lateral sin girar
- Mejor maniobrabilidad en espacios reducidos
- Posicionamiento preciso

### Sistema de Normalización
- Preserva dirección de movimiento
- Control suave y predecible
- Sin distorsión en movimientos combinados
- Respuesta consistente del robot

## Autor

**brianpech**  
Fecha de creación: 21 de Enero, 2026

## Notas de Desarrollo

### Versión Actual: MK1
- Sistema X-Drive omnidireccional con normalización implementado
- Control de intake básico funcional
- Rutina autónoma pendiente de implementación

### Próximas Mejoras
- [ ] Implementar rutina autónoma
- [ ] Agregar control de velocidad variable
- [ ] Implementar odometría para tracking de posición
- [ ] Agregar sistema de sensores

## Licencia

Proyecto de competencia VEX U - Uso educativo

---

**Última actualización**: Febrero 2026
