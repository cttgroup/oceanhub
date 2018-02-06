exports.SMALL = 768
exports.MEDIUM = 992
exports.PORT = 28002

if (window.location.hostname === '127.0.0.1') {
  exports.HOST = 'http://127.0.0.1:21018'
  exports.FRONT = 'http://127.0.0.1:21023'
} else {
  exports.HOST = 'http://ocean.cttgroup.ru:21018'
  exports.FRONT = 'http://ocean.cttgroup.ru:21023'
}


/*
class Example extends React.Component {
  constructor(props) {
    super(props)
    Cement.loadXHRData(url, function(data) {
        this.setState({data: data})
    }.bind(this))

    this.state = {
        data: [],
    }
  }

  componentDidMount() {
    console.log("DID MOUNT")
  }
  componentWillUnmount() {
    console.log("WILL UNMOUNT")
  }
  componentWillUpdate() {
    console.log("WILL UPDATE")
  }
  componentDidUpdate() {
    console.log("DID UPDATE")
  }
  componentWillReceiveProps(nextProps) {
    console.log("WILL RECEIVE PROPS", nextProps.myExampleProp)
  }

  render() {
      return (
          <div>
          </div>
      )
  }
}

// Specifies the types for props
Example.propTypes = {
  // name: PropTypes.string
}

// Specifies the default values for props
Example.defaultProps = {
  // name: 'Stranger'
}
*/


exports.XHRGet = function(url, callback) {
  var xhr = new XMLHttpRequest()
  xhr.responseType = 'json'
  xhr.open('GET', url, true)
  xhr.onreadystatechange = function() {
    if(xhr.readyState === 4) {
      // Request finished. Do processing here.
      if (xhr.status === 200 || xhr.status === 204) {
        // Got positive response
        if (callback && typeof(callback) === "function") {
          callback(xhr.response)
        }
      } else {
        // Got error
        console.log('Error: ' + xhr.status)
      }
    }
  }
  xhr.send()
}


exports.XHRPost = function(url, attributes, callback) {
  var xhr = new XMLHttpRequest()
  xhr.responseType = 'json'
  xhr.open("POST", url, true)
  xhr.setRequestHeader("Content-type", "application/x-www-form-urlencoded")
  xhr.onreadystatechange = function() {
    if(xhr.readyState === XMLHttpRequest.DONE) {
      // Request finished. Do processing here.
      if (xhr.status === 200 || xhr.status === 204) {
        // Got positive response
        if (callback && typeof(callback) === "function") {
          callback(xhr.response)
        }
      } else {
        // Got error
        console.log('Error: ' + xhr.status)
      }
    }
  }
  xhr.send(attributes)
}


exports.latlngsToWKT = function(latlngs) {
  let wkt = 'SRID=4326;MULTIPOLYGON('
  latlngs.forEach(function(polygon) {
    wkt +='('
    polygon.forEach(function(point) {
      wkt += point[0] + ' ' + point[1] + ', '
    })
    wkt +=')'
  })
  wkt +=')'
  return wkt
}


exports.WKTtoLatLngs = function(wkt) {
  let mpoly = []
  let latlngs = []
  mpoly = wkt.slice(22, -1).split('))')
  mpoly.forEach(function(element, key) {
    let poly = element.slice(4)
    if (poly.length > 0) {
      latlngs.push([])
      poly.split(', ').forEach(function(element) {
        let point  = element.split(' ')
        latlngs[key].push([point[1], point[0]])
      })
    }
  })

  return latlngs
}

