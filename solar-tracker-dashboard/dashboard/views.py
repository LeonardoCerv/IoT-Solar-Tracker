from django.shortcuts import render
from django.http import JsonResponse
from django.views.generic import TemplateView
from firebase_admin import db

# Create your views here.
def index(request):
    return render(request, 'interface/index.html')

def chart_data(request):
    ref = db.reference('chart_data')
    data = ref.get()
    return JsonResponse(data)

class IndexView(TemplateView):
    template_name = 'interface/index.html'



