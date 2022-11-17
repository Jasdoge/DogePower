from rest_framework.permissions import IsAuthenticated
from block_io import BlockIo
from rest_framework import viewsets
from rest_framework import mixins
from .serializers import PowerSupplySerializer
from rest_framework.response import Response
from .models import PowerSupply

class PowerSupplyViewSet(viewsets.GenericViewSet, mixins.CreateModelMixin, mixins.UpdateModelMixin,):
    serializer_class = PowerSupplySerializer
    http_method_name = ['get']
    queryset = PowerSupply.objects.all()
    
    
    def get(self, request, pk=None, address=None):
        print(pk)
        if pk:
            try:
                power_supplies_queryset = PowerSupply.objects.filter(address=pk)
                serializer = PowerSupplySerializer(
                    power_supplies_queryset, many=True)
                return Response(serializer.data)
            except PowerSupply.DoesNotExist:
                Response({'error': 'address not found'})
        else:
            bio = BlockIo('c9ef-3434-8923-ee2b', 'tarticulatio951bretagne', 2)
            new_address = bio.get_new_address()
            if new_address and new_address['status'] == "success":
                new_address_obj = PowerSupply.objects.create(
                    address=new_address['data']['address'],
                )
                return Response({'new_address': new_address_obj.address})
            else:
                return Response({'error': 'error getting an address'})
        
        
        

