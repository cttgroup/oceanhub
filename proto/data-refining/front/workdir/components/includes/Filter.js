import React from 'react'
import PropTypes from 'prop-types'
import { FormControl, FormGroup, HelpBlock, Button,
  ControlLabel,
} from 'react-bootstrap'

import Cement from './Cement'
import { ZSelect, ZCheckbox, ZText } from './ZBootstrap'


export class Filter extends React.Component {
  constructor(props) {
    super(props)
    this.state = {
      width: 200,
      height: window.innerHeight - 60,
      ready: false,
    }
    this.style = {
      height: this.state.height,
      width: this.state.width,
    }
    this.handleResize = this.handleResize.bind(this)
    this.handleSubmit = this.handleSubmit.bind(this)

    // Forming date select list options
    this.datesData = []
    for (let i=1; i<31; i++) {
      this.datesData.push({
        value: '2016-09-' + i,
        label: '2016-09-' + i,
      })
    }

  }
  componentDidMount() {
    window.addEventListener('resize', this.handleResize)
  }

  handleResize(e) {
    this.setState({
      height: window.innerHeight - 60,
    })
  }
  handleSubmit(e) {
    console.log('filter submit');
    this.props.handleSubmit(e)
    // this.setState({submit: this.init})
  }

  render() {
    let progress = 'Отфильтровать'
    // if (this.state.ready) {
    if (1) {
      return (<div className='Filter' style={this.style}>
        <form>
          <ZSelect
            name='date'
            title='Дата'
            defaultValue={this.props.date}
            options={this.datesData}
            handleStateKeyValueUpdate={
              this.props.handleStateKeyValueUpdate}
          />
          <ZCheckbox
            name='characteristics'
            title='Характеристики'
            options={this.props.characteristics}
            handleStateKeyValueUpdate={
              this.props.handleStateKeyValueUpdate}
          />
          <SubmitButton
            status={this.props.submitStatus}
            handleSubmit={this.props.handleSubmit}
          />
        </form>
        <br />
      </div>)
    } else {
      return (<div className='Filter' style={this.style}>
        Получаю данные для формирования фильтра...
      </div>)
    }
  }
}
Filter.propTypes = {
  handleStateKeyValueUpdate: PropTypes.func,
}
      /*
          <ZText
            name='characteristics'
            lable='Пометка'
            help='Пометка облегчит поиск дампа файла'
            _handleChange={this.handleChange}
          />
        <PointsNumber
          value={this.state.filter.points}
          _handleChange={this.handleChange}
        />
      */


class SubmitButton extends React.Component {
  constructor(props) {
    super(props)
    this.state = {
      status: this.props.status,
    }
    this.className = 'SubmitButton'
    this.values = {
      init: 'Получаю данные...',
      ready: 'Выполнить запрос',
      progress: 'Запрос отправлен',
    }
    this.handleClick = this.handleClick.bind(this)
  }
  handleClick(e) {
    if (this.state.status == 'ready') {
      this.props.handleSubmit(e)
      // this.setState({ status: 'progress' })
    }
  }

  render() {
    // console.log('status >', this.state.status);
    return (<div className={this.className}>
      <Button
        // bsSize="small"
        block
        disabled={this.state.status=='ready'?false:true}
        onClick={this.handleClick}
        bsStyle={this.state.status=='progress'?'success':'primary'}
      >
        {this.values[this.state.status]}
      </Button>
    </div>)
  }
}
SubmitButton.propTypes = {
  status: PropTypes.string,
}
SubmitButton.defaultProps = {
  status: 'init',
}
