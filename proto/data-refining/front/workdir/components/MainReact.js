import React from 'react'
import { Route } from 'react-router-dom'
import Cookie from 'js-cookie'

import MainMenu from './includes/MainMenu'
import { Home } from './Home'
import { Queries } from './Queries'
import { Auth } from './Auth'
import About from './About'


export class MainReact extends React.Component {
  constructor(props) {
    super(props)
    this.setUser = this.setUser.bind(this)
    this.state = {
      User: {
        token: (Cookie.get('token'))?Cookie.get('token'):null
      }
    }
  }

  setUser(data) {
    let User = this.state.User
    if (data) {
      User.token = 'blabla'
      Cookie.set('token', 'blabla', { expires: 7 })
      this.setState(User: User)
    } else {
      User.token = null
      Cookie.remove('token')
      this.setState(User: User)
    }
  }

  render() {
    if (this.state.User.token) {
      return (<div>
        <MainMenu />
        <Route exact path="/" component={Home}/>
        <Route path="/queries" component={Queries}/>
        <Route path="/about" component={About}/>
        <Route path="/auth" component={() => (<Auth
          User={this.state.User}
          _setUser={this.setUser}
        />)}/>
      </div>)
    } else {
      return (<div>
        <Auth
          User={this.state.User}
          _setUser={this.setUser}
        />
      </div>)
    }
  }
}
