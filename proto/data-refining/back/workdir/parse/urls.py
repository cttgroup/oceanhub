# parse
from django.conf.urls import url

from parse import views

urlpatterns = [
    url(r'^(?P<prefix>[\w-]+)/$', views.parse),
    url(r'^(?P<year>(19|20)\d{2})/(?P<day>\d+)/(?P<regionSlug>[\w-]+)/(?P<suffix>[\w-]+)/$', views.parse),
]
