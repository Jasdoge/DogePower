from rest_framework import serializers
from .models import PowerSupply 

class PowerSupplySerializer(serializers.ModelSerializer):

    class Meta:
        model = PowerSupply
        fields = ['address', 'time_unit', 'initial_time', 'start_time','minute_price', 'valid', 'paid', 'last_device_amount']
        
        
