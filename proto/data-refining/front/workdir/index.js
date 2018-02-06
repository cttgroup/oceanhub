import React from 'react'
import ReactDOM from 'react-dom'
import { BrowserRouter } from 'react-router-dom'

import { MainReact } from './components/MainReact'
import './css/leaflet.css'
import './css/scss.scss'


ReactDOM.render(
  <BrowserRouter>
    <MainReact />
  </BrowserRouter>,
  document.getElementById('mainReactMount')
)
