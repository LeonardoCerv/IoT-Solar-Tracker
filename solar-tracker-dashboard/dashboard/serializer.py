from rest_framework import serializers
from .models import PowerMeasurement

class PowerMeasurementSerializer(serializers.ModelSerializer):
    class Meta:
        model = PowerMeasurement
        fields = ['radiation', 'timestamp', 'electricity_generation']