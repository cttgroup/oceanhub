import React from 'react'
import { Button } from 'react-bootstrap'

import {
  ZFieldGroup
} from './includes/ZBootstrap'

import Cement from './includes/Cement'


export class Auth extends React.Component {
  constructor(props) {
    super(props)
    this.handleChange = this.handleChange.bind(this)
    this.handleSubmit = this.handleSubmit.bind(this)
    this.handleClear = this.handleClear.bind(this)
    this.state = {
      username: '',
      password: '',
      guest: false,
    }
  }

  handleClear() {
    this.props._setUser(null)
  }
  handleChange(key, value) {
    this.setState({[key]: value})
  }
  handleSubmit(obj) {
    obj.preventDefault()

    let attributes = "username=" + this.state.username
      + "&password=" + this.state.password
    Cement.XHRPost(
      Cement.HOST + '/ocean/api-token-auth/',
      attributes,
      data => {
        console.log('response', data);
        this.props._setUser(data)
      }
    )
  }

  render() {
    return (<div className='Auth'>
      <h1 className="post-title">Авторизация</h1>
      <form>
        <ZFieldGroup
          id="username"
          value={this.state.username}
          type="text"
          label="Логин"
          placeholder="Введите имя пользователя"
          _handleChange={this.handleChange}
        />
        <ZFieldGroup
          id="password"
          value={this.state.password}
          type="password"
          label="Пароль"
          placeholder="Введите пароль"
          _handleChange={this.handleChange}
        />

        <Login token={this.props.User.token}
          _handleSubmit={this.handleSubmit}
          _handleClear={this.handleClear}
        />
      </form>
    </div>)
  }
}
class Login extends React.Component {
  constructor(props) {
    super(props)
    this.handleSubmit = this.handleSubmit.bind(this)
  }

  handleSubmit(obj) {
    obj.preventDefault()
    this.props._handleSubmit(obj)
  }

  render() {
    if (this.props.token) {
      return (
        <Button
          bsStyle="primary"
          onClick={this.props._handleClear}
        >
          Выйти
        </Button>
      )
    } else {
      return (
        <Button
          type="submit"
          bsStyle="primary"
          onClick={this.handleSubmit}
        >
          Войти
        </Button>
      )
    }
  }
}
