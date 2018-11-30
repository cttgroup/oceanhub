const webpack = require('webpack')
const path = require('path')
const env = process.env.NODE_ENV


module.exports = {
  entry: './index.js',

  output: {
    path: path.join(__dirname, 'public'),
    filename: 'bundle.js',
    publicPath: '/'
  },

  plugins: env === 'production' ? [
    new webpack.optimize.OccurrenceOrderPlugin(),
    new webpack.DefinePlugin({
      'process.env': {
        NODE_ENV: JSON.stringify('production')
      }
    }),
    new webpack.optimize.UglifyJsPlugin(),
  ] : [],

  devtool: "source-map",

  module: {
    rules: [
      {
        test: /\.js$/,
        exclude: /(node_modules|bower_components)/,
        use: {
          loader: 'babel-loader',
          options: {
            presets: ['es2015', 'react'],
          }
        }
      },
      {
        test: /\.css$/,
        use: [ 'style-loader', 'css-loader' ]
      },
      {
        test: /\.(png|jpg|gif|svg|eot|ttf|woff|woff2)$/,
        loader: 'url-loader',
        options: {
          limit: 10000
        }
      },
      {
        test: /\.scss$/,
        use: [
          {
            loader: "style-loader"
          },
          {
            loader: "css-loader",
            options: {
              alias: {
                "../fonts/bootstrap": "bootstrap-sass/assets/fonts/bootstrap"
              }
            }
          },
          {
            loader: "sass-loader",
            options: {
              includePaths: [
                path.resolve("./node_modules/bootstrap-sass/assets/stylesheets")
              ]
            }
          }
        ]
      },
    ]
  }
}
