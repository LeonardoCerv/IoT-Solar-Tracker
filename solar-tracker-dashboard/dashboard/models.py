'''from django.db import models

class PowerMeasurement(models.Model):
    radiation = models.FloatField(help_text="Solar radiation measurement in W/m²")
    timestamp = models.DateTimeField(auto_now_add=True)
    
    @property
    def electricity_generation(self):
        return self.radiation * 0.20 #asumiendo eficiencia del 20%
    
    '''
