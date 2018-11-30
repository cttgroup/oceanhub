from django.conf.urls import include, url
from django.contrib import admin

from . import settings


urlpatterns = [
    url(r'^admin/', include(admin.site.urls)),
    url(r'^download/', include('download.urls')),
]

admin.site.site_header = settings.ADMIN_SITE_HEADER
admin.site.site_title = settings.ADMIN_SITE_TITLE
admin.site.index_title = settings.ADMIN_INDEX_TITLE
