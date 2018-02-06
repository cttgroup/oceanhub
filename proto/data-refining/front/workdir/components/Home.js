import React from 'react'

import Cement from './includes/Cement'
import { Filter } from './includes/Filter'
import { Map } from './includes/Map'

const isBrowser = typeof window !== 'undefined';
const L = isBrowser ? require('leaflet') : undefined

export class Home extends React.Component {
  constructor(props) {
    super(props)
    this.getRegionPositions = this.getRegionPositions.bind(this)
    this.state = {
      ready: false,
      data: [],

      // Map component props
      regionCenter: L.latLng(68, 6),
      defaultRegion: [],

      // Filter component props
      date: '2016-09-1',
      characteristics: [],
    }
    this.handleSubmit = this.handleSubmit.bind(this)
    this.handleStateKeyValueUpdate = this.handleStateKeyValueUpdate.bind(this)
  }
  componentDidMount() {
    // Update component state
    // this.setState({ })

    // Asynchronous loading default region data from backend
    Cement.XHRGet(
      Cement.HOST + '/ocean/api/regions/default/',
      function(data) {
        // Convert data in WKT format to multipolygon
        let defaultRegion = Cement.WKTtoLatLngs(data.mpoly)
        // Update component state
        this.setState({
          defaultRegion: defaultRegion,
        })
      }.bind(this)
    )

    // Asynchronous loading characteristics data from backend
    Cement.XHRGet(
      Cement.HOST + '/ocean/getfilter/',
      function(data) {
        // Forming characteristics checkbox options
        let characteristics = []
        data.characteristics.forEach(function(element) {
          if (element.model !== 'Bathymetric'
              && element.model !== 'SeaWindSpeed'
          ) {
            characteristics.push({
              label: element.title,
              value: element.model,
              checked:
                element.model!=='SeaSurfaceTemperature'?true:false,
            })
          }
        })
        // Update component state
        this.setState({
          characteristics: characteristics,
        })
      }.bind(this)
    )
  }

  // Get positions to render Leaflet polygon
  getRegionPositions(center, type) {
    type = (typeof type === 'undefined')?0:type

    let step = 1
    let path = []
    let x = center.lng
    let y = center.lat

    switch(type) {
      case -1:
        path = [[
        ],]
        break
      case 0:
        path = [[
          [y - step/2, x - step],
          [y + step/2, x - step],
          [y + step/2, x + step],
          [y - step/2, x + step],
        ],]
        break
      default:
        path = [[
        ],]
    }

    return path
  }

  handleSubmit(e) {
    // Get characteristics titles as array
    let characteristics = []
    this.state.characteristics.forEach(element => {
      if (element.checked) {
        characteristics.push(element.value)
      }
    })

    let submit = {
      // Get polygon by center and convert it to WKT
      region: Cement.latlngsToWKT(
        this.getRegionPositions(this.state.regionCenter)),
      date: this.state.date,
      characteristics: characteristics,
    }

    // Sending POST request to backend with submit array data
    Cement.XHRPost(
      Cement.HOST + '/dump/create/',
      JSON.stringify(submit),
      function(response) {
        // window.location.href = '/queries'
      }
    )
  }
  handleStateKeyValueUpdate(key, value) {
    console.log('state key value update: ' + key + ' ' + value);
    // Update component state by given key and value
    this.setState({[key]: value})
  }

  render() {
    // Cement.AuthCheck()
    return (<div>
      <Filter
        characteristics={this.state.characteristics}
        handleSubmit={this.handleSubmit}
        handleStateKeyValueUpdate={this.handleStateKeyValueUpdate}
        submitStatus='ready'
      />
      <Map
        defaultRegion={this.state.defaultRegion}
        regionPositions={this.getRegionPositions(this.state.regionCenter)}
        handleStateKeyValueUpdate={this.handleStateKeyValueUpdate}
      />
    </div>)
  }
}

