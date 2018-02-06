/*
- вывод произвольных табличных данных
- редактирование произвольной табличной строки
- обновление по index на .find
- замена bind(this)
- сортировка
- передача colNumbers
*/
import React, {Component} from 'react'
import PropTypes from 'prop-types'

import {
  Table, Modal, Col,
  Form, FormGroup, FormControl, ControlLabel,
  Button
} from 'react-bootstrap'


export class MegaTable extends Component {
  constructor(props) {
    super(props)
    this.state = {
      data: this.props.data,
    }
  }
  componentWillReceiveProps(nextProps) {
    this.setState({data: nextProps.data})
  }

  render() {
    console.log('legend', this.state.data.legend);
    let legend = this.state.data.legend
    let thead = <tr><td></td></tr>
    thead = Object.keys(legend).map((element, key) =>
      <td key={key}>{legend[element]}</td>
    )

    let rows = this.state.data.body.map((row, key) => {
      return (
        <tr key={key}>{
          Object.keys(legend).map((element, key) =>
          {
          // console.log('legend', element);
          // console.log('rowel',row.find(rowElement => rowElement == element))
          // console.log('row', row[element]);
            return (<td key={key}>{row[element]}</td>)
          })
        }</tr>
      )
    })


    return(<div>
      <Table striped bordered condensed hover>
        <thead>
          <tr>
            {thead}
          </tr>
        </thead>
        <tbody>
        {rows}
        </tbody>
      </Table>
    </div>)
  }
}
MegaTable.propTypes = {
  data: PropTypes.object.isRequired,
}



export class ZMegaTable extends Component {
  constructor(props) {
    super(props);
    this.state = {
      data: this.props.data,
      body: [],
      legend: [],
      colNumbers: -1,

      arrHeader: [],
      arrBody: [],
      modalData: {
        index: -1,
        element: {}
      },
      modalEdit: false,
    };
    this.handleModalSubmit = this.handleModalSubmit.bind(this)
  }
  componentWillReceiveProps(nextProps) {
    // this.setState({body: nextProps.data.body})
  }
  /*
  componentWillUpdate(nextProps, nextState) {
    // this.body = nextState.body
  }
  */

  handleModalSubmit(values, index) {
    let arrBody = this.state.arrBody
    arrBody[index] = values
    this.setState({arrBody: arrBody, modalEdit: false})
  }

  fetchDetails(element, index) {
    this.setState({
      modalData:{
        index: index,
        element: element
      }
    })
    this.setState({modalEdit: true});
    console.log("index", index);
  }

  renderResultRows(data) {
    return data.map((element, index) => {
      return (
        <tr id={index} key={index} onClick={() => this.fetchDetails(element, index)}>
          <td data-title="ID">{element.ID}</td>
          <td data-title="sCode">{element.sCode}</td>
          <td data-title="sName">{element.sName}</td>
          <td data-title="sComment">{element.sComment}</td>
        </tr>
      );
    });  // no need to bind with anon function
  }

  render() {
    console.log('data', this.state.data);
    return (<div>
      <h1>Справочник списочных значений</h1>
      <Table striped bordered condensed hover>
        <thead>
          <tr>
            <th>{this.state.arrHeader.length>0? this.state.arrHeader[0].ID: ""}</th>
            <th>{this.state.arrHeader.length>0? this.state.arrHeader[0].sCode: ""}</th>
            <th>{this.state.arrHeader.length>0? this.state.arrHeader[0].sName: ""}</th>
            <th>{this.state.arrHeader.length>0? this.state.arrHeader[0].sComment: ""}</th>
          </tr>
        </thead>
        <tbody>
          {this.state.body}
        </tbody>
      </Table>
      <TableRowEdit
        modalEdit={this.state.modalEdit}
        data={this.state.modalData}
        _handleModalSubmit={this.handleModalSubmit}
      />
    </div>)
  }
}

class TableRowEdit extends React.Component {
  constructor(props) {
    super(props);
    this.state = {
      showModal: this.props.modalEdit,
      index: -1,
      values: {},
    }
    this.close = this.close.bind(this);
    this.open = this.open.bind(this);
    this.handleChange = this.handleChange.bind(this)
    this.handleSubmit = this.handleSubmit.bind(this)
  }

  componentWillReceiveProps(nextProps) {
    this.setState({
      showModal: nextProps.modalEdit,
      index: nextProps.data.index,
      values: nextProps.data.element,
    });
  }

  handleSubmit() {
    console.log("index",this.state.index);
    this.props._handleModalSubmit(this.state.values, this.state.index)
  }

  close() {
    this.setState({showModal: false });
  }

  open() {
    this.setState({showModal: true });
  }

  handleChange(event) {
    let values=this.state.values
    values[event.target.name]=event.target.value
    this.setState({values: values});
  }

  render() {
    return (<div className="static-modal">
      <Modal show={this.state.showModal} onHide={this.close}>
        <Modal.Header>
          <Modal.Title>
            Идентификатор: {this.props.data.element.ID}
          </Modal.Title>
        </Modal.Header>

        <Modal.Body>
        <Form horizontal>
        <FormGroup controlId="formHorizontalsCode">
          <Col componentClass={ControlLabel} sm={3}>
            Код:
          </Col>
          <Col sm={9}>
            <FormControl
              name="sCode"
              type="text"
              value={typeof this.state.values.sCode === "undefined"?
                "" : this.state.values.sCode}
              placeholder="Введите текст"
              onChange={this.handleChange}
            />
          </Col>
        </FormGroup>
        <FormGroup controlId="formHorizontalsName">
          <Col componentClass={ControlLabel} sm={3}>
            Наименование:
          </Col>
          <Col sm={9}>
            <FormControl
              name="sName"
              type="text"
              value={typeof this.state.values.sName === "undefined"?
                "" : this.state.values.sName}
              placeholder="Введите текст"
              onChange={this.handleChange}
            />
          </Col>
        </FormGroup>
        </Form>

        </Modal.Body>

        <Modal.Footer>
          <Button onClick={this.close}>Close</Button>
          <Button bsStyle="primary" onClick={this.handleSubmit}>Save changes</Button>
        </Modal.Footer>

      </Modal>
    </div>
    )
  }
}


export default MegaTable


      /*
       table: { colNumbers: -1, body: [], legend: {}, },
        let table = {
          body: data,
          legend: {
            id: 'ID',
            timestamp: 'Время создания',
            status: 'Статус',
            file_name: 'Файл',
            file_size: 'Размер',
            elapsed_time: 'Время выполнения',
          },
          columns: [{
              Header: 'ID',
              accessor: 'id'
            }, {
              Header: 'Время создания',
              accessor: 'timestamp',
          }],
        }
        */
