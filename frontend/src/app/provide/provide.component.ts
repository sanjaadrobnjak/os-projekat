import { Component, inject, OnInit } from '@angular/core';
import { HelperService } from '../services/helper.service';
import { Service } from '../models/Service';
import { Router } from '@angular/router';

@Component({
  selector: 'app-provide',
  standalone: true,
  imports: [],
  templateUrl: './provide.component.html',
  styleUrl: './provide.component.css'
})
export class ProvideComponent implements OnInit{
  ngOnInit(): void {
    this.service.getAllServices().subscribe(
      (services: Service[]) => {
        this.allServices = services;
      })
  }
  private service = inject(HelperService)
  private router = inject(Router)
  allServices: Service[] = []

  chooseService(id: number){
    this.service.chooseService(id).subscribe(data=>{
      this.router.navigate([`/viewservice`], {
        state: {service: data}
      });
      alert(data.name)
    })
  }
  
}
