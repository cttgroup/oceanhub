import React from 'react'
import {
  // BrowserRouter as Router,
  // Route,
  Link
} from 'react-router-dom'
import { Nav,
  Navbar,
  NavItem,
} from 'react-bootstrap'


var links = [
    {'link': '/queries', 'title': 'Запросы'},
    {'link': '/auth', 'title': 'Авторизация'},
    {'link': '/about', 'title': 'О проекте'},
    {'link': 'https://cttgroup.github.io/oceanhub/', 'title': 'Github'},
]

class MainMenu extends React.Component {
  constructor(props) {
    super(props)
    this.state = {
    }
  }
  render() {
    let rowsLinks = []
    links.forEach(function(prop, key) {
      let className = ''
      if (typeof prop.className !== 'undefined') {
          className = prop.className
      }
      rowsLinks.push(<MenuLink
        key={key}
        link={prop.link}
        title={prop.title}
        className={className}
                     />)
    })
              //<img src="/images/logosm.png" />

    return (<div className="Nav">
      <Navbar staticTop inverse>
        <Navbar.Header>
          <Navbar.Brand>
            <Link
              to="/"
            >
              Океан
            </Link>
          </Navbar.Brand>
          <Navbar.Toggle />
        </Navbar.Header>
        <Navbar.Collapse>
          <Nav>
            {rowsLinks}
          </Nav>
        </Navbar.Collapse>
      </Navbar>
    </div>)
  }
            //<Navbar fixedTop={true}>
}

/*
          <Nav>
            <NavItem
              // eventKey={this.props.key}
              href=''
              className=''
            >
              Авторизация
            </NavItem>
          </Nav>
*/


class MenuLink extends React.Component {
  render() {
    return (
      <NavItem
        eventKey={this.props.key}
        href={this.props.link}
        className={this.props.className}
      >
        {this.props.title}
      </NavItem>
    )
  }
}


export default MainMenu
