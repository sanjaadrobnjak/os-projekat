import { HttpClient } from '@angular/common/http';
import { inject, Injectable } from '@angular/core';
import { Service } from '../models/Service';

@Injectable({
  providedIn: 'root'
})
export class HelperService {
  http = inject(HttpClient)
  backPath = "http://localhost:8080"
  constructor() { }

  getAllServices(){
    return this.http.get<Service[]>(`${this.backPath}/getAllServices`)
  }

  chooseService(id: number){
    alert(id)
    const data={
      id: id
    }
    return this.http.post<Service>(`${this.backPath}/getService`, data)
  }
}
