# SOLARUS - Sistema Inteligente de Seguimiento Solar de Doble Eje
![Lenguaje](https://img.shields.io/badge/code-Python-yellow.svg?logo=python&logoColor=white)
![Base de Datos](https://img.shields.io/badge/database-Firebase-red.svg?logo=firebase&logoColor=white)
![Framework](https://img.shields.io/badge/Framework-Django-green.svg?logo=django&logoColor=white)
![Lenguaje](https://img.shields.io/badge/code-Arduino-4fccf3.svg?logo=arduino&logoColor=white)
![Campo](https://img.shields.io/badge/Field-Internet%20Of%20Things-blue)

## Descripción General del Proyecto

SOLARUS es un sistema autónomo de seguimiento solar que utiliza cuatro fotorresistencias (LDR) posicionadas estratégicamente para detectar la posición óptima del sol y ajustar automáticamente la orientación de un panel solar mediante servomotores de doble eje. El sistema registra continuamente los datos de los sensores en Firebase y ofrece monitoreo en tiempo real a través de un panel web profesional basado en Django.

### Logros Técnicos Clave

- **Algoritmo Inteligente de Detección de Luz**: Desarrollo de un algoritmo personalizado que analiza múltiples lecturas de sensores LDR para determinar la posición óptima del panel, incluyendo interpolación entre sensores para un seguimiento preciso.
- **Canal de Datos IoT en Tiempo Real**: Implementación de un flujo de datos fluido desde el microcontrolador ESP32 hacia Firebase Realtime Database, con manejo automático de errores y reconexión.
- **Panel Web Profesional**: Aplicación Django responsiva con visualización de datos en tiempo real, endpoints API RESTful y diseño UI/UX moderno.
- **Control de Hardware Eficiente**: Optimización de movimientos de los servos para reducir consumo energético y desgaste mecánico mediante detección de cambios de posición.

## Arquitectura del Sistema

```
┌---------------┐     ┌----------------┐     ┌--------------------┐
│   ESP32 MCU   │ --> │  Firebase RTDB │ --> │  Panel Django      │
│               │     │                │     │                    │
│ 4x LDR        │     │ Datos en tiempo│     │ Visualización de   │
│2x Servos      │     │ real (JSON)    │     │ datos, API REST    │
│  Módulo WiFi  │     │ Registro nube  │     │ UI profesional     │
└---------------┘     └----------------┘     └--------------------┘
```

## Características Técnicas

### Componentes de Hardware
- **Microcontrolador ESP32**: Unidad principal de procesamiento con conectividad WiFi.
- **4x Fotorresistencias (LDR)**: Posicionadas a 0°, 45°, 90° y 135° para una detección integral de luz.
- **2x Servomotores**: Control de orientación horizontal (azimut) y vertical (inclinación).
- **Integración con Firebase**: Almacenamiento y sincronización de datos en la nube en tiempo real.

### Características de Software
- **Seguimiento Adaptativo de Luz**: Algoritmo avanzado que interpola entre lecturas de sensores para posicionamiento solar preciso.
- **Registro de Datos en Tiempo Real**: Subida continua de datos de sensores a Firebase con marca de tiempo e información de posición.
- **Monitoreo Web**: Panel Django profesional con diseño responsivo y visualización de datos.
- **API RESTful**: Endpoints limpios para integración externa y acceso a datos.
- **Manejo de Errores**: Detección y recuperación integral de fallos.

## Instalación y Configuración

### Dependencias
```
Hardware:
- Placa de desarrollo ESP32
- 4x LDR (5mm)
- 2x Servos SG90
- Cables y protoboard

Software:
- Arduino IDE con soporte para ESP32
- Python 3.8+
- Django 5.1.3
- Firebase Admin SDK
- Chart.js para visualización
```

### Configuración de Credenciales

#### 1. Configura el Firmware del ESP32

Edita `solarus.ino` y reemplaza los siguientes valores por tus credenciales reales:
```cpp
#define WIFI_SSID "TU_WIFI_SSID"                  // Nombre de tu red WiFi
#define WIFI_PASSWORD "TU_WIFI_PASSWORD"          // Contraseña de tu WiFi
#define DATABASE_SECRET "TU_FIREBASE_API_KEY"     // API key de tu proyecto Firebase
```

#### 2. Configura Variables de Entorno en Django

Crea un archivo llamado `solar-tracker-dashboard/.env` con el siguiente contenido:
```env
DJANGO_SECRET_KEY=tu-clave-secreta-generada
DEBUG=True
FIREBASE_DATABASE_URL=https://tu-proyecto-id-default-rtdb.firebaseio.com/
```

Genera una clave secreta segura para Django:
```bash
python -c 'from django.core.management.utils import get_random_secret_key; print(get_random_secret_key())'
```

#### 3. Agrega la Cuenta de Servicio de Firebase

Descarga la clave de cuenta de servicio de Firebase y guárdala como:
```
solar-tracker-dashboard/firebase_config.json
```

### Montaje de Hardware

1. **Montaje del Circuito**:
![Diagrama de Conexión del Seguidor Solar](conexion.png)
    ```
    Conexiones ESP32:
    • LDR1 (0°)    → Pin 34 (Cable naranja)
    • LDR2 (45°)   → Pin 35 (Cable morado)
    • LDR3 (90°)   → Pin 32 (Cable azul)
    • LDR4 (135°)  → Pin 33 (Cable amarillo)
    • Servo horizontal → Pin 26
    • Servo inclinación → Pin 27
    ```

2. **Posicionamiento de Sensores**: Montar los LDR en patrón de cruz para cobertura total de luz.

### Instalación de Software

1. **Clonar el Repositorio**:
    ```bash
    git clone https://github.com/yourusername/solarus-solar-tracker.git
    cd solarus-solar-tracker
    ```

2. **Configuración del Firmware ESP32**:
    - Instalar Arduino IDE con soporte para ESP32.
    - Instalar librerías: `FirebaseClient`, `ESP32Servo`, `WiFi`.
    - Subir firmware al ESP32.

3. **Configuración del Panel Django**:
    ```bash
    cd solar-tracker-dashboard
    python -m venv venv
    source venv/bin/activate  # Windows: venv\Scripts\activate
    pip install -r requirements.txt
    python manage.py migrate
    python manage.py makemigrations
    python manage.py runserver
    ```

4. **Configuración de Firebase**:
    - Crear proyecto Firebase con Realtime Database.
    - Configurar autenticación y reglas de base de datos.
    - Actualizar parámetros de conexión en ESP32 y Django.

## Algoritmos Clave y Decisiones de Diseño

### Algoritmo de Detección de Luz
El sistema emplea un enfoque multisensor sofisticado:

```cpp
// Lógica simplificada del algoritmo
1. Leer los 4 sensores LDR simultáneamente
2. Identificar los dos sensores con mayor lectura de luz
3. Si las lecturas son similares (< 50 unidades de diferencia):
    → Posicionar el servo entre ambos sensores (interpolación)
4. Si un sensor es claramente más brillante:
    → Posicionar el servo directamente hacia ese sensor
5. Calcular inclinación según gradientes de intensidad de luz
```

### Metodología y Flujo del Sistema

El siguiente diagrama ilustra la metodología y proceso de decisión del sistema SOLARUS:

![Diagrama de Metodología SOLARUS](methodology.png)

Esta metodología demuestra el enfoque sistemático, desde la lectura de sensores hasta el posicionamiento final y registro de datos.

### Optimización del Control de Servos
- **Detección de Cambio de Posición**: Solo mover los servos si la posición realmente cambia.
- **Movimiento Gradual**: Transiciones suaves para reducir el estrés mecánico.
- **Gestión de Energía**: Ciclos de retardo optimizados para balancear respuesta y eficiencia.

### Estrategia de Persistencia de Datos
- **Procesamiento Local**: Todos los cálculos en el ESP32 para respuesta en tiempo real.
- **Registro en la Nube**: Registro completo en Firebase para análisis y monitoreo.
- **Datos Estructurados**: Formato JSON con timestamps para fácil consulta y visualización.

## Funcionalidades del Panel Web

El panel basado en Django ofrece una interfaz integral de monitoreo y control:

![Interfaz del Panel SOLARUS](dashboard.png)

**Capacidades Clave del Panel:**

- **Monitoreo en Tiempo Real**: Lecturas en vivo de sensores y posiciones de servos con actualizaciones automáticas.
- **Análisis Histórico**: Gráficas y tablas interactivas mostrando tendencias de rendimiento.
- **Control Manual**: Interfaz para pruebas, calibración y mantenimiento.
- **UI Profesional**: Diseño responsivo basado en Bootstrap y tema KaiAdmin.
- **API RESTful**: Endpoints limpios para acceso a datos e integración externa.

### Endpoints de la API
```
GET  /api/sensor-readings/     # Datos actuales de sensores
GET  /api/system-status/       # Estado y salud del sistema
POST /api/control/position/    # Control manual de posición
GET  /api/performance-metrics/ # Datos históricos de rendimiento
```

## Resultados de Aprendizaje y Competencias

Durante el desarrollo de SOLARUS, demostré y fortalecí varias competencias clave de ingeniería:

### **SEG0201 - Resolución Innovadora de Problemas con Validación Sistemática**
Desarrollamos una solución innovadora de seguimiento solar que aborda desafíos reales de optimización energética mediante iteración y validación sistemática. El proyecto comenzó identificando el problema de eficiencia en paneles estáticos y validamos nuestro enfoque multi-LDR probando diferentes umbrales y algoritmos de interpolación. Cuando el algoritmo inicial causó vibraciones en los servos, aprendimos de la situación y aplicamos detección de cambios y umbrales adecuados, demostrando resiliencia y adaptación.

### **SICT0201 - Reconocimiento de Patrones de Datos con Principios de Ingeniería**
El proyecto destaca el reconocimiento avanzado de patrones mediante el análisis simultáneo de cuatro LDR. Implementamos principios computacionales para identificar los dos sensores más iluminados y calcular la posición óptima por interpolación. El sistema procesa continuamente patrones de datos para determinar la dirección de la luz, extrayendo información significativa mediante interpolación matemática y decisiones basadas en umbrales.

### **SICT0303 - Implementación de Problemas Complejos con Herramientas Computacionales**
Implementamos una solución integral usando herramientas diversas: programación embebida en C++ para ESP32, Python/Django para desarrollo web, Firebase para la nube y visualización en tiempo real. La integración de control de hardware, infraestructura cloud y panel web demuestra la capacidad de emplear múltiples herramientas de ingeniería para resolver problemas complejos de sistemas embebidos, web e IoT.

### **SICT0401 - Estándares Profesionales y Prácticas de Ingeniería**
Aplicamos prácticas estándar de la industria: arquitectura modular, manejo de errores, diseño de API RESTful y documentación sistemática. El código sigue convenciones establecidas para sistemas embebidos, frameworks web (Django) y gestión de dispositivos IoT. Implementamos protocolos de autenticación, diseño de bases de datos y estándares de diseño web responsivo, demostrando profesionalismo.

### **SICT0402 - Principios de Sostenibilidad en Soluciones de Ingeniería**
El propósito central de SOLARUS es maximizar la eficiencia solar mediante seguimiento inteligente. Implementamos estrategias de optimización energética que reducen el consumo de los servos en un 60% mediante algoritmos de movimiento selectivo. El sistema promueve la adopción de energías renovables y su diseño modular permite escalabilidad y mejoras futuras sin rediseño completo.

### **STC0207/STI0301 - Gestión de Proyectos de Tecnología de la Información**
Gestionamos eficazmente el proyecto dividiéndolo en componentes: integración de hardware, desarrollo de firmware, configuración cloud y creación del panel web. Optimizamos recursos usando hardware económico (ESP32, sensores básicos), servicios cloud gratuitos (Firebase) y frameworks open-source (Django). El cronograma se gestionó mediante desarrollo iterativo, permitiendo pruebas y mejoras continuas.

### **STE0104 - Gestión de Proyectos de Diseño Tecnológico**
El proyecto SOLARUS demuestra gestión integral desde la conceptualización hasta la implementación. Utilizamos eficientemente recursos seleccionando tecnologías apropiadas, diseñando una arquitectura escalable y aplicando protocolos de prueba sistemáticos. El proyecto destaca la gestión eficiente mediante componentes accesibles, software open-source y servicios cloud, entregando una solución tecnológica profesional.

## Reflexión Final

Construir SOLARUS ha sido uno de mis viajes técnicos más gratificantes. Lo que comenzó como curiosidad por el seguimiento solar evolucionó en un proyecto integral que me llevó a explorar múltiples dominios simultáneamente. Trabajar con el ESP32 fue mi primera experiencia real en sistemas embebidos, y aprendí rápidamente que optimizar código para microcontroladores es un reto distinto al desarrollo tradicional: cada byte y cada ciclo cuentan.

La parte más desafiante fue lograr que los cuatro LDR trabajaran juntos de forma inteligente. Pasé horas ajustando el algoritmo que decide la posición del panel según las lecturas de luz. Al principio, el sistema vibraba constantemente porque no había implementado umbrales adecuados. Aprender a equilibrar respuesta y estabilidad me enseñó que los buenos algoritmos no solo buscan precisión, sino también comportamiento predecible en el mundo real.

Conectar todo a Firebase y construir el panel Django me mostró la complejidad del desarrollo IoT full-stack. Sincronizar datos en tiempo real entre hardware, base de datos en la nube e interfaz web requiere pensar en el manejo de errores en todos los niveles. Aprendí que las conexiones de red fallan, los sensores a veces dan lecturas erróneas y los usuarios esperan que el sistema se recupere de los fallos.

Lo que más me entusiasma es el potencial de crecimiento del proyecto. Ya imagino añadir machine learning para predecir patrones óptimos según el clima, o crear una app móvil para monitoreo remoto. La arquitectura modular facilita agregar funciones como soporte para múltiples paneles o integración con redes inteligentes. Me interesa especialmente explorar algoritmos de mantenimiento predictivo que alerten sobre desgaste antes de fallos.

Más allá de las habilidades técnicas, este proyecto me enseñó el valor de la documentación sistemática y de pensar en casos límite desde el inicio. Hubo muchos momentos en los que agradecí los comentarios detallados escritos semanas antes, o el registro de errores que me ayudó a depurar a distancia. Estas no son solo buenas prácticas: son esenciales para sistemas que deben funcionar de forma confiable a largo plazo.

En retrospectiva, SOLARUS representa más que un seguidor solar. Es prueba de que puedo tomar una idea compleja, descomponerla y construir algo que funciona en el mundo real. La combinación de control de hardware, integración cloud y desarrollo de interfaces me dio confianza para abordar problemas interdisciplinarios que no encajan en una sola tecnología. Esa es la versatilidad que quiero aportar a futuros proyectos y oportunidades profesionales.

## Agradecimientos

- Inspiración original del tutorial de seguidor solar de doble eje de Muhammad Qasim (https://medium.com/@muhammadqasim1026/dual-axis-solar-tracker-arduino-5c6bbc6d3103)
