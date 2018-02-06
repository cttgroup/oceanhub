## ocean_backdjango
import os


# Quick-start development settings - unsuitable for production
# See https://docs.djangoproject.com/en/1.9/howto/deployment/checklist/

# Build paths inside the project like this: os.path.join(BASE_DIR, ...)
BASE_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
# SECURITY WARNING: keep the secret key used in production secret!
SECRET_KEY = 'e0d*&8sl01Sf(v2d=gs99-9vqvu3mw*033oow_(!az7l$p)7u#'
ALLOWED_HOSTS = ['127.0.0.1', 'localhost', 'frontnodejs', ]

# SECURITY WARNING: don't run with debug turned on in production!
# Setting DEBUG flag in depence from environment variable
try:
    if os.environ['DJANGO_ENV'] == 'production':
        DEBUG = False
except:
        DEBUG = True



# Application definition
INSTALLED_APPS = [
    'django.contrib.admin',
    'django.contrib.auth',
    'django.contrib.contenttypes',
    'django.contrib.sessions',
    'django.contrib.messages',
    'django.contrib.staticfiles',
    'django.contrib.gis',

    # Plugging aditional applications
    'rest_framework',
    'rest_framework.authtoken',
    'corsheaders',

    # Plugging own applications
    'ocean',
    'parse',
    'dump',
]


MIDDLEWARE_CLASSES = [
    'django.middleware.security.SecurityMiddleware',
    'django.contrib.sessions.middleware.SessionMiddleware',
    'django.middleware.common.CommonMiddleware',
    'django.middleware.csrf.CsrfViewMiddleware',
    'django.contrib.auth.middleware.AuthenticationMiddleware',
    'django.contrib.auth.middleware.SessionAuthenticationMiddleware',
    'django.contrib.messages.middleware.MessageMiddleware',
    'django.middleware.clickjacking.XFrameOptionsMiddleware',

    # Django CORS
    'corsheaders.middleware.CorsMiddleware',

    # Caching
    # 'django.middleware.cache.CacheMiddleware',
    # 'django.middleware.cache.UpdateCacheMiddleware',
]


ROOT_URLCONF = 'app.urls'


TEMPLATES = [
    {
        'BACKEND': 'django.template.backends.django.DjangoTemplates',
        'DIRS': [],
        'APP_DIRS': True,
        'OPTIONS': {
            'context_processors': [
                'django.template.context_processors.debug',
                'django.template.context_processors.request',
                'django.contrib.auth.context_processors.auth',
                'django.contrib.messages.context_processors.messages',
            ],
        },
    },
]


WSGI_APPLICATION = 'app.wsgi.application'


# Database
# https://docs.djangoproject.com/en/1.9/ref/settings/#databases
DATABASES = {
    'default': {
         'ENGINE': 'django.contrib.gis.db.backends.postgis',
         'NAME': 'ocean',
         'USER': 'ocean',
         'HOST': 'backpostgis',
         'PORT': 5432,
    },
}

# Password validation
# https://docs.djangoproject.com/en/1.9/ref/settings/#auth-password-validators
AUTH_PASSWORD_VALIDATORS = [
    {
        'NAME': 'django.contrib.auth.password_validation'
        '.UserAttributeSimilarityValidator',
    },
    {
        'NAME': 'django.contrib.auth.password_validation'
        '.MinimumLengthValidator',
    },
    {
        'NAME': 'django.contrib.auth.password_validation'
        '.CommonPasswordValidator',
    },
    {
        'NAME': 'django.contrib.auth.password_validation'
        '.NumericPasswordValidator',
    },
]


REST_FRAMEWORK = {
    'DEFAULT_FILTER_BACKENDS': (
        'django_filters.rest_framework.DjangoFilterBackend',
    ),
    'DEFAULT_AUTHENTICATION_CLASSES': (
        'rest_framework.authentication.TokenAuthentication',
    )
}

CACHES = {
    'default': {
        'BACKEND': 'django.core.cache.backends.memcached.PyLibMCCache',
        'LOCATION': 'backmemcached:11211',
    }
}
"""
"""

# Internationalization
# https://docs.djangoproject.com/en/1.9/topics/i18n/
LANGUAGE_CODE = 'ru-ru'
TIME_ZONE = 'UTC'
USE_I18N = True
USE_L10N = True
# USE_TZ = True
USE_TZ = False


# Static files (CSS, JavaScript, Images)
# https://docs.djangoproject.com/en/1.9/howto/static-files/
STATIC_URL = '/static/'
# URL_STATIC_ROOT = os.path.join(BASE_DIR, 'static')
STATICFILES_DIRS = (
    # Using with build-in web-server
    # os.path.join(BASE_DIR, 'static'),
)
# Using with web-server
STATIC_ROOT = os.path.join(BASE_DIR, "static/")


MEDIA_URL = '/media/'
MEDIA_ROOT = os.path.join(BASE_DIR, "media/")


# CORS stuff
CORS_ORIGIN_ALLOW_ALL = True
"""
CORS_ORIGIN_WHITELIST = (
    '127.0.0.1:3000',
    '127.0.0.1:5000',
)
"""


# Directory for storing data corresponding to docker-compose.yml
DATA_DIR = '/data/'


# Admin localization settings
ADMIN_SITE_HEADER = "Океан - аналитическая система"
ADMIN_SITE_TITLE = "Океан - аналитическая система"
ADMIN_INDEX_TITLE = "Управление данными"


# Celery stuff
BROKER_URL = 'redis://backredis:6379'
CELERY_RESULT_BACKEND = 'redis://backredis:6379'
CELERY_ACCEPT_CONTENT = ['application/json']
CELERY_TASK_SERIALIZER = 'json'
CELERY_RESULT_SERIALIZER = 'json'
CELERY_TIMEZONE = 'Europe/Moscow'


# Posgres driver import
try:
    import psycopg2
except ImportError:
    # Fall back to psycopg2cffi
    from psycopg2cffi import compat
    compat.register()

# Try to import local container settings from /local/settings.py
try:
    from app.rginx_settings.py import *
except ImportError:
    pass

# Try to import local settings from local_settings.py
try:
    from app.local_settings import *
except ImportError:
    pass
