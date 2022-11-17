from rest_framework.permissions import IsAuthenticated
from block_io import BlockIo
from rest_framework import viewsets
from rest_framework import mixins
from .serializers import PowerSupplySerializer
from rest_framework.response import Response
from django.utils import timezone
from .models import PowerSupply

import decimal

class PowerSupplyViewSet(viewsets.GenericViewSet, mixins.CreateModelMixin, mixins.UpdateModelMixin,):
    serializer_class = PowerSupplySerializer
    http_method_names = ['get', 'post']
    queryset = PowerSupply.objects.all()
    
    
    def get(self, request, pk=None, address=None):
        bio = BlockIo('fb72-091c-44f6-fc80', 'tarticulatio951bretagne', 2)
        if pk:
            print(pk)
            try:
                power_supplies_queryset = PowerSupply.objects.filter(address=pk)
                serializer = PowerSupplySerializer(
                    power_supplies_queryset, many=True)

                last_power_supply = power_supplies_queryset.filter(valid=True).last()
                if last_power_supply:
                    print(last_power_supply.address)
                    wallet_amount = decimal.Decimal(bio.get_address_balance(addresses='2NBjqKrcwyh8Ce5FXbZnaXZeyN1WuNFmMoU')['data']['available_balance'])
                    print(wallet_amount)
                    price = last_power_supply.minute_price * 60 * last_power_supply.initial_time if last_power_supply.time_unit == 'H' \
                        else last_power_supply.minute_price * 60 * 12 * last_power_supply.initial_time
                    print(price)
                    if wallet_amount > last_power_supply.last_device_amount + price: # check diff
                        last_power_supply.last_device_amount += wallet_amount
                        last_power_supply.start_date = timezone.now()
                        last_power_supply.paid = True
                        last_power_supply.save()

                return Response(serializer.data)
            except PowerSupply.DoesNotExist:
                Response({'error': 'address not found'})
        else:
            new_address = bio.get_new_address()
            if new_address and new_address['status'] == "success":
                new_address_obj = PowerSupply.objects.create(
                    address=new_address['data']['address'],
                    #last_device_amount=bio.get_address_balance('%s' % new_address)['data']['available_balance']
                )
                return Response({'new_address': new_address_obj.address})
            else:
                return Response({'error': 'error getting an address'})

    def post(self, request, pk=None, address=None, test=None):
        print(self.request.query_params.__dict__)
        print(address)
        print(test)
        new_power_supply = PowerSupply.objects.create(address=pk)
        power_supplies_queryset = PowerSupply.objects.filter(address=pk)
        last_power_supply = power_supplies_queryset.filter(valid=True).last()
        if last_power_supply:
            print('*', last_power_supply.last_device_amount)
            new_power_supply.last_device_amount = last_power_supply.last_device_amount
            new_power_supply.save()
        serializer = PowerSupplySerializer(
            power_supplies_queryset, many=True)
        return Response(serializer.data)
        
        

