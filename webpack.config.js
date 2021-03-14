const devMode = process.env.NODE_ENV !== 'production';
const webpack = require("webpack");
const HtmlWebpackPlugin = require("html-webpack-plugin");
const MiniCssExtractPlugin = require("mini-css-extract-plugin");
const CssMinimizerPlugin = require('css-minimizer-webpack-plugin');

module.exports = {
    entry: "./index.js",
    output: {
        path: __dirname + "/build",
        filename: "bundle.js"
    },
    module: {
        rules: [
            { test: /\.css$/, use: [MiniCssExtractPlugin.loader, "css-loader"] }
        ]
    },
    optimization: {
        minimize: true,
        minimizer: [
            `...`,
            new CssMinimizerPlugin({
                minimizerOptions: {
                    preset: [
                        "default",
                        {
                            discardComments: { removeAll: true }
                        }
                    ]
                }
            })
        ]
    },
    plugins: [
        new MiniCssExtractPlugin({ filename: "styles.css" }),
        new HtmlWebpackPlugin({
            title: "WC的小玩具",
            template: "./index.html",
            filename: "index.html",
            inject: false,
            minify: true
        }),
        new HtmlWebpackPlugin({
            title: "RGBLight Management",
            template: "./rgblight.html",
            filename: "rgblight.html",
            inject: false,
            minify: true
        })
        // TODO 有时间模块化
    ],
    // devtool: "eval-source-map",
    devServer: {
        contentBase: "./build",
        historyApiFallback: true,
        hot: true,
        inline: true,
        progress: true
    }
};
