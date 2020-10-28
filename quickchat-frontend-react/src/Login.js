import React, { Component } from 'react';

class Login extends Component {

    loginHandle = (evt) => {
        evt.preventDefault();
    }

    render() {
        return (
            <div>
                <form onSubmit={this.loginHandle}>
                    <input type='text'></input>
                    <input type='password'></input>
                    <input type='submit'></input>
                </form>
            </div>
        );
    }
}

export default Login;