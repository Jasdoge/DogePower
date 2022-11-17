from rest_framework import serializers
from .models import PowerSupply 

class PowerSupplySerializer(serializers.ModelSerializer):

    class Meta:
        model = PowerSupply
        fields = ['address', 'payment_expires']
        
        
