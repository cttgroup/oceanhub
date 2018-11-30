import React from 'react'
import PropTypes from 'prop-types'
import { FormControl, FormGroup, HelpBlock, Button,
  ControlLabel, Checkbox,
} from 'react-bootstrap'


export class ZSelect extends React.Component {
  constructor(props) {
    super(props)
    this.options = []
    this.state = {
      select: 'norv-small'
    }
    this.handleChange = this.handleChange.bind(this)

    // Forming options list array
    this.props.options.forEach(function(element, key) {
      this.options.push(
        <option
          key={key}
          value={element.value}
          selected={element.selected}
        >
          {element.value}
        </option>
      )
    }.bind(this))
  }

  handleChange(obj) {
    // Buble date changes to the parent component
    this.props.handleStateKeyValueUpdate(this.props.name, obj.target.value)
    this.setState({value: obj.target.value})
  }

  render() {
    return (<div>
      <FormGroup controlId="formControlsSelect">
        <ControlLabel>{this.props.title}</ControlLabel>
        <FormControl
          componentClass="select"
          defaultValue={this.props.defaultValue}
          value={this.props.value}
          placeholder="select"
          onChange={this.handleChange}
        >
          {this.options}
        </FormControl>
      </FormGroup>
    </div>)
  }
}


export class ZFieldGroup extends React.Component {
  constructor(props) {
    super(props)
    this.handleChange = this.handleChange.bind(this)
  }

  handleChange(obj) {
    let func = this.props._handleChange
    if (typeof func === 'function') {
      func(this.props.id, obj.target.value)
    }
  }

  render() {
    return (
      <FormGroup controlId={this.props.id}>
        <ControlLabel>{this.props.label}</ControlLabel>
        <FormControl
          value={this.props.value}
          type={this.props.type}
          placeholder={this.props.placeholder}
          onChange={this.handleChange}
        />
        {this.props.help && <HelpBlock>{this.props.help}</HelpBlock>}
      </FormGroup>
    )
  }
}


export class ZCheckbox extends React.Component {
  constructor(props) {
    super(props)
    this.handleChange = this.handleChange.bind(this)
  }

  handleChange(obj) {
    let options = this.props.options
    const find = (value) => options.find((element, index, array) => {
      if (element.value == value) {
        console.log('index > ', index, element);
        array[index].checked = !array[index].checked
      }
    })
    find(obj.target.value)
    this.props.handleStateKeyValueUpdate('characteristics', options)
  }

  render() {
    // Forming checkbox list array
    let rows = []
    this.props.options.forEach(function(element, key) {
      rows.push(
        <Checkbox
          key={key}
          value={element.value}
          onChange={this.handleChange}
          checked={element.checked}
        >
          {element.label}
        </Checkbox>
      )
    }.bind(this))

    return (
      <FormGroup>
        <ControlLabel>{this.props.title}</ControlLabel>
        {rows}
      </FormGroup>
    )
  }
}
ZCheckbox.propTypes = {
  handleStateKeyValueUpdate: PropTypes.func,
}


export class ZSingleCheckbox extends React.Component {
  constructor(props) {
    super(props)
    this.options = []
    this.state = {
      checked: this.props.checked,
    }
    this.handleChange = this.handleChange.bind(this)
  }

  handleChange(obj) {
    let func = this.props._handleChange
    if (typeof func === 'function') {
      func(this.props.id, obj.target.checked)
    }
  }

  render() {
    return (
      <Checkbox
        key={this.props.id}
        checked={this.props.checked}
        onChange={this.handleChange}
      >
        {this.props.lable}
      </Checkbox>
    )
  }
}


export class ZText extends React.Component {
  constructor(props) {
    super(props)
    this.state = {
      value: '',
    }
    this.getValidationState = this.getValidationState.bind(this)
    this.handleChange = this.handleChange.bind(this)
  }

  handleChange(obj) {
    let func = this.props._handleChange
    if (typeof func === 'function') {
      func(this.props.name, obj.target.value)
    }
    this.setState({ value: obj.target.value })
  }
  getValidationState() {
    const length = this.state.value.length;
    if (length > 10) return 'success';
    else if (length > 5) return 'warning';
    else if (length > 0) return 'error';
  }

  render() {
    return (
      <FormGroup
        controlId={this.props.name}
        // validationState={this.getValidationState()}
      >
        <ControlLabel>{this.props.label}</ControlLabel>
        <FormControl
          type="text"
          value={this.state.value}
          placeholder="Введите текст"
          onChange={this.handleChange}
        />
        <FormControl.Feedback />
        <HelpBlock>{this.props.help}</HelpBlock>
      </FormGroup>
    )
  }
}


const FormExample = React.createClass({



  render() {
    return (
      <form>
      </form>
    );
  }
});
