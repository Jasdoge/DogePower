from rest_framework.permissions import IsAuthenticated
from rest_framework import viewsets
from rest_framework import mixins
from .serializers import PowerSupplySerializer
from rest_framework.response import Response
from .models import PowerSupply

class PowerSupplyViewSet(viewsets.GenericViewSet, mixins.CreateModelMixin, mixins.UpdateModelMixin,):
    serializer_class = PowerSupplySerializer
    permission_classes = (IsAuthenticated,)
    http_method_name = ['get']
    queryset = PowerSupply.objects.all()
    
    
    def get(self, request, pk, address=None):
        power_supplies_queryset = PowerSupply.objects.all()
        serializer = PowerSupplySerializer(
            power_supplies_queryset, many=True)
        print(pk)
        print(address)
        return Response(serializer.data)        	
        
        
        

