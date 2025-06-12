
from django.core.management.base import BaseCommand
from dashboard.models import PowerMeasurement
from datetime import datetime, timedelta
import random

class Command(BaseCommand):
    help = 'Initialize the database with sample power measurements and sensor data'

    def add_arguments(self, parser):
        parser.add_argument('--count', type=int, default=100, help='Number of records to create')

    def handle(self, *args, **options):
        count = options['count']
        
        # Delete existing records
        PowerMeasurement.objects.all().delete()
        
        # Generate sample data
        base_time = datetime.now() - timedelta(days=7)
        
        for i in range(count):
            timestamp = base_time + timedelta(minutes=i*10)
            radiation = random.uniform(0, 1000)  # Random value between 0 and 1000 W/m²
            
            PowerMeasurement.objects.create(
                radiation=radiation,
                timestamp=timestamp
            )
            
        self.stdout.write(self.style.SUCCESS(f'Successfully created {count} power measurements'))