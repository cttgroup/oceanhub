from django.conf.urls import include, url
from django.contrib import admin
from django.views.static import serve


from . import settings


urlpatterns = [
    url(r'^admin/', include(admin.site.urls)),
    url(r'^world/', include('world.urls')),
    url(r'^ocean/', include('ocean.urls')),
    url(r'^parse/', include('parse.urls')),
    url(r'^dump/', include('dump.urls')),

    url(r'^media/(?P<path>.*)$', serve, {
        'document_root': settings.MEDIA_ROOT,
    }),

    # Serving files in static directory
    # url(r'^static/(?P<path>.*)$', django.views.static.serve,
        # {'document_root': settings.STATIC_ROOT}),
]

admin.site.site_header = settings.ADMIN_SITE_HEADER
admin.site.site_title = settings.ADMIN_SITE_TITLE
admin.site.index_title = settings.ADMIN_INDEX_TITLE
