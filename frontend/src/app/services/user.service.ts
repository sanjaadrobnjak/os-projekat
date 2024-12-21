import { HttpClient } from '@angular/common/http';
import { inject, Injectable } from '@angular/core';
import { User } from '../models/User';

@Injectable({
  providedIn: 'root'
})
export class UserService {
  http = inject(HttpClient)
  backPath = "http://localhost:8080"
  constructor() { }

  register(u: User){
    const data={
      firstname: u.firstname,
      lastname: u.lastname,
      username: u.username,
      password: u.password,
      address: u.address,
      mail: u.mail,
      type: u.type,
      subscription: u.subscription
    }
    return this.http.post<User>(`${this.backPath}/register`, data)
  }

  login(username: string, password: string){
    const data = {
      username: username,
      password: password,
    }
    return this.http.post<User>(`${this.backPath}/login`, data)

  }

  changePassword(username: string, password: string){
    const data = {
      username: username,
      password: password,
    }
    return this.http.post<User>(`${this.backPath}/changePassword`, data)
  }
}
