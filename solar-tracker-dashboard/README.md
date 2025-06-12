# Solar Tracker Dashboard

A comprehensive IoT solution for monitoring and controlling a dual-axis solar panel tracking system built with ESP32 and Django.

## Project Structure

```
solar-tracker-dashboard/
├── manage.py                           # Django management script
├── config/                            # Django project configuration
│   ├── __init__.py
│   ├── settings.py                    # Main settings
│   ├── urls.py                        # URL routing
│   ├── wsgi.py                        # WSGI configuration
│   └── asgi.py                        # ASGI configuration
├── dashboard/                         # Main Django application
│   ├── __init__.py
│   ├── apps.py                        # App configuration
│   ├── models.py                      # Database models
│   ├── views.py                       # View controllers
│   ├── urls.py                        # App URL routing
│   ├── serializer.py                  # API serializers
│   ├── admin.py                       # Admin interface
│   ├── tests.py                       # Unit tests
│   ├── management/                    # Custom management commands
│   │   └── commands/
│   │       └── initialize_sample_data.py
│   ├── static/                        # Static assets
│   │   └── assets/
│   │       ├── stylesheets/           # CSS files
│   │       ├── scripts/               # JavaScript files
│   │       ├── images/                # Image assets
│   │       └── fonts/                 # Font files
│   └── templates/                     # HTML templates
│       └── dashboard/
│           └── index.html             # Main dashboard
└── README.md                          # This file
```

## Features

- **Real-time Monitoring**: Live solar panel performance metrics
- **Interactive Dashboard**: Modern web interface with responsive design
- **Data Visualization**: Charts and graphs for power generation trends
- **System Control**: Manual solar panel positioning controls
- **Firebase Integration**: Real-time database for IoT data synchronization
- **REST API**: RESTful endpoints for data access
- **Professional UI**: Built with Bootstrap 5 and Kaiadmin theme

## Technology Stack

### Backend
- **Django 5.1.3**: Python web framework
- **Django REST Framework**: API development
- **SQLite**: Local database
- **Firebase Admin SDK**: Cloud database integration

### Frontend
- **HTML5/CSS3/JavaScript**: Core web technologies
- **Bootstrap 5**: UI framework
- **Chart.js**: Data visualization
- **Kaiadmin Theme**: Professional admin template
- **Firebase SDK**: Real-time data updates

### Hardware Integration
- **ESP32**: Microcontroller for solar tracking
- **Firebase Realtime Database**: IoT data storage
- **Servo Motors**: Panel positioning
- **LDR Sensors**: Light intensity measurement

## Installation

1. **Clone the repository**
   ```bash
   git clone <repository-url>
   cd solar-tracker-dashboard
   ```

2. **Create virtual environment**
   ```bash
   python -m venv venv
   source venv/bin/activate  # On Windows: venv\Scripts\activate
   ```

3. **Install dependencies**
   ```bash
   pip install django djangorestframework firebase-admin
   ```

4. **Configure Firebase**
   - Create a Firebase project
   - Download the service account key as `firebase_config.json`
   - Place it in the project root directory
   - Update the database URL in `config/settings.py`

5. **Run migrations**
   ```bash
   python manage.py migrate
   ```

6. **Initialize sample data (optional)**
   ```bash
   python manage.py initialize_sample_data
   ```

7. **Run the development server**
   ```bash
   python manage.py runserver
   ```

8. **Access the dashboard**
   Open your browser and navigate to `http://127.0.0.1:8000`

## Hardware Setup

1. **ESP32 Configuration**
   - Flash the `solarus.ino` code to your ESP32
   - Configure WiFi credentials and Firebase settings
   - Connect LDR sensors and servo motors as per the circuit diagram

2. **Sensor Placement**
   - Position 4 LDR sensors in cardinal directions (N, E, S, W)
   - Mount servo motors for horizontal and vertical panel movement
   - Ensure proper power supply for all components

## API Endpoints

The dashboard provides RESTful API endpoints for data access:

- `GET /api/power-measurements/` - Retrieve power generation data
- `GET /api/sensor-readings/` - Get current sensor values
- `POST /api/control/` - Send control commands to the tracker

## Development

### Adding New Features

1. **Models**: Define data structures in `dashboard/models.py`
2. **Views**: Add controllers in `dashboard/views.py`
3. **Templates**: Create HTML templates in `dashboard/templates/dashboard/`
4. **Static Files**: Add assets to appropriate `dashboard/static/assets/` subdirectories
5. **URLs**: Configure routing in `dashboard/urls.py`

### Static File Organization

- **Stylesheets**: `dashboard/static/assets/stylesheets/`
- **Scripts**: `dashboard/static/assets/scripts/`
- **Images**: `dashboard/static/assets/images/`
- **Fonts**: `dashboard/static/assets/fonts/`

## Deployment

For production deployment:

1. **Security Settings**
   - Set `DEBUG = False` in `config/settings.py`
   - Configure `ALLOWED_HOSTS`
   - Use environment variables for secrets

2. **Static Files**
   ```bash
   python manage.py collectstatic
   ```

3. **Database**
   - Configure production database in settings
   - Run migrations on production server

4. **Web Server**
   - Use WSGI server like Gunicorn
   - Configure reverse proxy with Nginx

## Contributing

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Test thoroughly
5. Submit a pull request

## License

This project is licensed under the MIT License.

## Support

For questions or issues, please create an issue in the repository or contact the development team.
