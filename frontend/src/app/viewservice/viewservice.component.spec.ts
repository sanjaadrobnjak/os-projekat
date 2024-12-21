import { ComponentFixture, TestBed } from '@angular/core/testing';

import { ViewserviceComponent } from './viewservice.component';

describe('ViewserviceComponent', () => {
  let component: ViewserviceComponent;
  let fixture: ComponentFixture<ViewserviceComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      imports: [ViewserviceComponent]
    })
    .compileComponents();

    fixture = TestBed.createComponent(ViewserviceComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
