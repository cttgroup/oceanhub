import React from 'react'
import ReactTable from 'react-table'
import 'react-table/react-table.css'

import Cement from './includes/Cement'


export class Queries extends React.Component {
  constructor(props) {
    super(props)
    this.state = {
      table: { colNumbers: -1, body: [], legend: {}, },
      tasks1: [],
      tasks2: [],
    }
  }
  componentDidMount() {
    // Async get queries data
    Cement.XHRGet(
      Cement.HOST + '/dump/get/',
      function(data) {
        let table = {
          body: data,
          columns: [{
              id: 'status',
              Header: 'Статус',
              accessor: 'status',
              Cell: props => <span className='number'>{
                props.value === '1'?'Обрабатывается':'Выполнен'
              }</span>,
            }, {
              Header: 'ID',
              accessor: 'id'
            }, {
              Header: 'Время создания',
              accessor: 'timestamp',
            }, {
              Header: 'Файл',
              accessor: 'file_name',
              Cell: props => <span className='link'>
                <a href={Cement.HOST + "/media/data/json/" + props.value}>{props.value}</a>
              </span>,
            }, {
              Header: 'Размер',
              accessor: 'file_size',
            }, {
              Header: 'Время выполнения',
              accessor: 'elapsed_time',
          }],
        }
        this.setState({
          table: table,
        })
      }.bind(this)
    )
  }

  render() {
    return (<div className='Queries'>
      <h1 className="post-title">Запросы</h1>
      {this.state.table.columns === undefined?(
        <div>Загрузка данных</div>
      ):(
        <ReactTable
          data={this.state.table.body}
          columns={this.state.table.columns}
        />
      )}
    </div>)
  }
}
