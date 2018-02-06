import React from 'react'
import PropTypes from 'prop-types'
// import Cement from './Cement'

const isBrowser = typeof window !== 'undefined';
const RL = isBrowser ? require('react-leaflet') : undefined
// const L = isBrowser ? require('leaflet') : undefined
// const RLC = isBrowser ? require('react-leaflet-control') : undefined
// const handler = isBrowser ? require('leaflet-path-drag') : undefined



export class Map extends React.Component {
  constructor(props) {
    super(props)
    this.state = {
      status: 'init',
      width: window.innerWidth,
      height: window.innerHeight - 61,
    }
    this.position = [61.505, -0.09]

    // Define mapbox tile layer
    this.id = 'sergeysynergy.cigwfojr8008nstlykqyvldtv'
    this.defaultTileLayerURL = 'https://api.tiles.mapbox.com/v4/{id}/{z}/{x}/{y}.png?access_token=pk.eyJ1Ijoic2VyZ2V5c3luZXJneSIsImEiOiJjaWd3Zm9qejUwMDlydnFrcmloeTBwOW9vIn0.4JdGQv9pO-z4ncyf-DO4Ow'

    this.handleClick = this.handleClick.bind(this)
    this.handleResize = this.handleResize.bind(this)
  }
  componentDidMount() {
    // Adding window resize event listener
    window.addEventListener('resize', this.handleResize)

    // Moving leaflet map controls
    this.map.leafletElement.zoomControl.setPosition('topright')
  }

  handleClick(e) {
    // Buble region center changes to the parent component
    this.props.handleStateKeyValueUpdate('regionCenter', e.latlng)
  }
  handleResize(e) {
    this.setState({
      width: window.innerWidth,
      height: window.innerHeight - 61,
    })
  }

  render() {
    let mapStyle = { height: this.state.height }

    return (<div className="Map">
      <RL.Map
        ref={(Map) => { this.map = Map }}
        center={this.position}
        zoom={3}
        style={mapStyle}
        onClick={this.handleClick}
      >
        { /* Default region with points, using mapbox as tile layer */}
        <RL.TileLayer
          url={this.defaultTileLayerURL}
          id={this.id}
        />

        { /* Region defined by user */}
        <Region
          positions={this.props.regionPositions}
          handleStateKeyValueUpdate={this.props.handleStateKeyValueUpdate}
        />
      </RL.Map>
    </div>)
  }
}
      /*
      <h3>{this.state.status}</h3>

        <RL.Polygon
          positions={this.props.defaultRegion}
          color={'lightblue'}
        />

        // Using mapbox as tile layer
        <RL.TileLayer
          url={this.defaultTileLayerURL}
          id={this.id}
        />
        */


class Region extends React.Component {
  constructor(props) {
    super(props)
    this.handleDragEnd = this.handleDragEnd.bind(this)
  }

  // Handle Region component dragging
  handleDragEnd() {
    // Buble path to parent component
    this.props.handleStateKeyValueUpdate(
      'regionCenter', this.proxy.leafletElement.getCenter())
  }

  render() {
    return (<RL.Polygon
      positions={this.props.positions}
      ref={(Polygon) => { this.proxy = Polygon }}
      color={this.props.color}
      draggable={true}
      onDragEnd={this.handleDragEnd}
      className='Region'
    />)
  }
}
Region.propTypes = {
  handleStateKeyValueUpdate: PropTypes.func,
}
Region.defaultProps = {
  color: 'blue',
}
